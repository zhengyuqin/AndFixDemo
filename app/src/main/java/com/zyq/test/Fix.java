package com.zyq.test;

import android.util.Log;

import com.alipay.euler.andfix.Util_LOGFile;

/**
 * @author zyq 15-9-24
 */
public class Fix {
	private static String TAG = "euler";
	String s = "s";
	private static O o = new O("fix");
	static int i = 11;

	public static String a(String str) {
		Util_LOGFile.getInstance().write(TAG + "fix success");
		Log.d(TAG, "fix success");
		return "b";
	}

	public int b(String s1, String s2) {
		Util_LOGFile.getInstance().write(TAG + s1);
		Util_LOGFile.getInstance().write(TAG + "fix success" + s);
		Log.d(TAG, s1);
		Log.d(TAG, "fix success " + s);
		return 0;
	}

	public String c() {
		return "a";
	}
}
