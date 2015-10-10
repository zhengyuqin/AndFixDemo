package com.zyq.andfixdemo;

import android.app.Application;
import android.os.Environment;
import android.util.Log;
import android.widget.Toast;

import com.alipay.euler.andfix.Util_LOGFile;
import com.alipay.euler.andfix.patch.PatchManager;

import java.io.File;
import java.io.IOException;

/**
 * @author zyq 15-9-24
 */
public class MyApplication extends Application {
	private static final String TAG = "euler";

	private static final String APATCH_PATCH = "out.apatch";
	private static final String LOG_NAME = "andfixlog.txt";

	private PatchManager mPatchManager;

	@Override
	public void onCreate() {
		super.onCreate();


		String logFilePath = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + LOG_NAME;
		Util_LOGFile.getInstance().open(logFilePath);

		mPatchManager = new PatchManager(this);
		mPatchManager.init("1.0");

		Util_LOGFile.getInstance().write(TAG + "\n" + "inited. ");
		Log.d(TAG, "inited. ");

		mPatchManager.loadPatch();
		Util_LOGFile.getInstance().write(TAG + "\n" + "apatch loaded");
		Log.d(TAG, "apatch loaded");

		try {
			String patchFileString = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator +
					APATCH_PATCH;


			mPatchManager.addPatch(patchFileString);
			File file = new File(patchFileString);
			if (file != null && file.exists()) {
				Util_LOGFile.getInstance().write(TAG + "\n" + "文件存在，大小为为：" + file.length());
				Toast.makeText(this, "文件大小为:" + file.length(), Toast.LENGTH_LONG).show();
			}
			Util_LOGFile.getInstance().write(TAG + "\n" + "apatch:" + patchFileString + " added. ");
			Log.d(TAG, "apatch:" + patchFileString + " added. ");
		} catch (IOException e) {
			Util_LOGFile.getInstance().write(TAG + "\n" + "" + e.toString());
			Log.d(TAG, e.toString());
		}
	}
}
