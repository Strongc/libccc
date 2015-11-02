#include "MessageLoop.h"
#include "internal/Common_internal.h"
#include "Lock.h"
#include "Semaphore.h"
#include "Utiles.h"
#include <queue>
#include <vector>
#include <map>
#include <algorithm>

namespace ccc {

namespace internal_ {

	struct MessageLoopData {
		typedef std::queue<Message> message_queue_type;
		typedef std::map<void*, MessageLoopBase::dispatcher_type> dispatcher_map_type;
		typedef std::vector<MessageLoopBase::filter_type> filter_queue_type;
		
		Mutex mtxQueueMessage;
		message_queue_type queueMessage;

		Mutex mtxMapDispatcher;
		dispatcher_map_type mapDispatcher;
		
		Mutex mtxQueueFilter;
		filter_queue_type queueFilter;
		
		Atom atmStop;
		Semaphore semMessage;
		Mutex mtxLooping;
	};
}

Atom Message::s_atm(0);

Message::Message() : from(0), to(0), loop(0) {
	this->seq = s_atm.inc();
}

Message::Message(const Message& other) {
	this->seq = other.seq;
	this->from = other.from;
	this->to = other.to;
	this->loop = other.loop;
	this->data = other.data;
}

Message::Message(const std::string& type) : from(0), to(0), loop(0) {
	this->seq = s_atm.inc();
	this->type = type;
}

Message::Message(const std::string& type, const Any& data) : from(0), to(0), loop(0) {
	this->seq = s_atm.inc();
	this->type = type;
	this->data = data;
}

Message& Message::operator =(const Message& other) {
	if (&other == this) return *this;

	this->seq = other.seq;
	this->from = other.from;
	this->to = other.to;
	this->loop = other.loop;
	this->data = other.data;

	return *this;
}

typedef std::queue<Message> message_queue_type;
typedef std::map<void*, MessageLoopBase::dispatcher_type> dispatcher_map_type;
typedef std::vector<MessageLoopBase::filter_type> filter_queue_type;

const void* MessageLoopBase::MESSAGE_TARGET_NONE = 0;
const void* MessageLoopBase::MESSAGE_TARGET_BROADCAST = (void*)-1;

MessageLoop::MessageLoop() {
	pd_ = new internal_::MessageLoopData;
}

MessageLoop::MessageLoop(internal_::MessageLoopData* pd) {
	CCC_ASSERT(pd);
	pd_ = pd;
}

MessageLoop::~MessageLoop() {
	delete pd_;
}

bool MessageLoop::registerDispatcher(void* toId, MessageLoopBase::dispatcher_type dispatcher) {
	if (toId == MESSAGE_TARGET_BROADCAST ||
		toId == MESSAGE_TARGET_NONE) {
		return false;
	}
	
	CCC_LOCK(pd_->mtxMapDispatcher);
	
	pd_->mapDispatcher[toId] = dispatcher;
	
	return true;
}

void MessageLoop::unregisterDispatcher(void* toId) {
	if (toId == MESSAGE_TARGET_BROADCAST ||
		toId == MESSAGE_TARGET_NONE) {
		return;
	}
	
	CCC_LOCK(pd_->mtxMapDispatcher);
	
	dispatcher_map_type::iterator it = pd_->mapDispatcher.find(toId);
	
	if (it != pd_->mapDispatcher.end()) {
		pd_->mapDispatcher.erase(it);
	}
}

bool MessageLoop::post(Message msg) {
	if (msg.from == MESSAGE_TARGET_BROADCAST) {
		return false;
	}
	
	msg.loop = this;
	
	CCC_LOCK(pd_->mtxQueueMessage);

	pd_->queueMessage.push(msg);
	pd_->semMessage.post();
	
	return true;
}

bool MessageLoopBase::postTo(void* toId, void* fromId, Message msg) {
	msg.from = fromId;
	msg.to = toId;

	int r = this->post(msg);

	int a = 1;

	return r;
}

bool MessageLoopBase::postback(Message msg) {
	CCC_ASSERT(msg.loop == this);

	void* temp = msg.from;
	
	msg.from = msg.to;
	msg.to = temp;
	
	return this->post(msg);
}

bool MessageLoopBase::postback(Message msg, void* rewriteFromId) {
	msg.to = rewriteFromId;
	return postback(msg);
}

void MessageLoop::installFilter(filter_type filter) {
	CCC_LOCK(pd_->mtxQueueFilter);
	pd_->queueFilter.push_back(filter);
}

void MessageLoop::uninstallFilter(filter_type filter) {
	CCC_LOCK(pd_->mtxQueueFilter);
	
	pd_->queueFilter.erase(std::remove(pd_->queueFilter.begin(), pd_->queueFilter.end(), filter),
		pd_->queueFilter.end());
}

void MessageLoop::exec() {
	CCC_LOCK(pd_->mtxLooping);

	for (;;) {
		if (execOnce_internal()) {
			return;
		}
	}
}

bool MessageLoop::execOnce() {
	CCC_LOCK(pd_->mtxLooping);
	return execOnce_internal();
}

bool MessageLoop::execOnce_internal() {
	pd_->semMessage.wait();

	if (pd_->atmStop.get()) {
		return true;
	}
	
	Message msg;

	{ // 获取消息
		CCC_LOCK(pd_->mtxQueueMessage);
		
		if (pd_->queueMessage.empty()) {
			return (pd_->atmStop.get() != 0);
		}

		msg = pd_->queueMessage.front();
		pd_->queueMessage.pop();
	}
	
	{ // 过滤消息
		CCC_LOCK(pd_->mtxQueueFilter);

		bool ignore = false;

		for (filter_queue_type::iterator it = pd_->queueFilter.begin();
			it != pd_->queueFilter.end(); ++it) {
			(*it)(msg, ignore);
			
			unsigned stop = pd_->atmStop.get();
			
			if (ignore || stop) {
				return (stop != 0);
			}
		}
	}
	
	if (msg.to == MESSAGE_TARGET_NONE) {
		// 内部处理消息
		this->defaultDispatcher(msg);
		return (pd_->atmStop.get() != 0);
	}

	{ // 分派消息
		CCC_LOCK(pd_->mtxMapDispatcher);

		if (msg.to == MESSAGE_TARGET_BROADCAST) {
			// 广播消息
			for (dispatcher_map_type::iterator it = pd_->mapDispatcher.begin();
				it != pd_->mapDispatcher.end(); ++it) {
				if (pd_->atmStop.get()) {
					return true;
				}

				it->second(msg);
			}
		} else {
			dispatcher_map_type::iterator it = pd_->mapDispatcher.find(msg.to);
			
			if (it != pd_->mapDispatcher.end()) {
				it->second(msg);
			}
		}
	}
	
	return (pd_->atmStop.get() != 0);;
}

void MessageLoop::stop() {
	stopAsync();

	{
		// stop获得锁时，exec或execOnce一定已经返回
		CCC_LOCK(pd_->mtxLooping);
	}
}

void MessageLoop::stopAsync() {
	pd_->atmStop.set(1);
	pd_->semMessage.post();
}

void MessageLoop::defaultDispatcher(Message msg) {
	// TODO: log
}

}