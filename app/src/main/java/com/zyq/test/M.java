package com.zyq.test;

import com.alipay.euler.andfix.Util_LOGFile;

/**
 * @author zyq 15-10-10
 */
public class M {

	private static final String TAG = "euler";
	private int a = 10;

	public M() {
	}


	public int setData( int data) {
		a = data;
		Util_LOGFile.getInstance().write(TAG + "\n数据为" + a);
		return a;
	}
}
