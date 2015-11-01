#ifndef __CCC_MESSAGE_LOOP_H__
#define __CCC_MESSAGE_LOOP_H__

#include "Common.h"
#include "Any.h"
#include "RefPtr.h"
#include "Bind.h"
#include <string>

namespace ccc {

	namespace internal_ {
		struct MessageLoopData;
	}
	
	struct MessageLoopBase;

	class Message {
	public:
		void* from;
		void* to;
		MessageLoopBase* loop;

		std::string type;
		unsigned seq;
		Any data;
		
		Message();
		Message(const std::string& type);
		Message(const std::string& type, const Any& data);

	private:
		static Atom s_atm;
	};
	
	struct MessageLoopBase {
		typedef Proc1<Message> dispatcher_type;
		typedef Proc2<Message&, bool&> filter_type;
		
		static const void* MESSAGE_TARGET_BROADCAST;
		static const void* MESSAGE_TARGET_NONE;

		virtual bool registerDispatcher(void* toId, dispatcher_type dispatcher) = 0;
		virtual void unregisterDispatcher(void* toId) = 0;
		virtual bool post(Message msg) = 0;
		virtual void installFilter(filter_type filter) = 0;
		virtual void uninstallFilter(filter_type filter) = 0;
		virtual void exec() = 0;
		virtual bool execOnce() = 0;
		virtual void stop() = 0;
		virtual void stopAsync() = 0;
		virtual void defaultDispatcher(Message msg) = 0;

		bool post(void* fromId, void* toId, Message msg);
		bool postback(Message msg);
		bool postback(Message msg, void* rewriteFromId);
	};
	
	class MessageLoop : public MessageLoopBase {
	public:
		MessageLoop();
		explicit MessageLoop(internal_::MessageLoopData* pd);
		virtual ~MessageLoop();

		virtual bool registerDispatcher(void* toId, dispatcher_type dispatcher);
		virtual void unregisterDispatcher(void* toId);

		virtual bool post(Message msg);

		virtual void installFilter(filter_type filter);
		virtual void uninstallFilter(filter_type filter);

		virtual void exec();
		virtual bool execOnce();
		
		virtual void stop();
		virtual void stopAsync();

		virtual void defaultDispatcher(Message msg);

	protected:
		virtual bool execOnce_internal();

		internal_::MessageLoopData* pd_;
	};
}

#endif // __CCC_MESSAGE_LOOP_H__