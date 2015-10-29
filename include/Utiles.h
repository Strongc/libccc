#ifndef __CCC_UTILES_H__
#define __CCC_UTILES_H__

namespace ccc {

	/**
	 * 当前线程睡眠
	 * @param ms 睡眠毫秒数，范围[0, 0xffffffff/1000)
	 */
	void msleep(unsigned ms);

	/**
	 * 获取当前时钟
	 * @return 时钟值
	 */
	unsigned getTicks();

	/**
	 * 获取唯一id
	 * @return id值
	 */
	unsigned getId();
}

#endif // __CCC_UTILES_H__