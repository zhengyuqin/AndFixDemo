package com.zyq.test;

import android.util.Log;

import com.alipay.euler.andfix.Util_LOGFile;

/**
 * @author zyq 15-9-24
 */
public class A {
	private static String TAG = "euler";
	String s = "s";
	private static O o = new O("a");
	static int i = 10;

	public static String a(String str) {
		Util_LOGFile.getInstance().write(TAG + "fix error");
		Log.e(TAG, "fix error");
		return "a";
	}

	public int b(String s1, String s2) {
		Util_LOGFile.getInstance().write(TAG + "fix error");
		Util_LOGFile.getInstance().write(TAG + o.s);
		Log.e(TAG, "fix error");
		Log.e(TAG, o.s);
		return 0;
	}

	public int getI() {
		return i;
	}
}
