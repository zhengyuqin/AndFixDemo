package com.alipay.euler.andfix;

import android.text.TextUtils;
import android.util.Log;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * @author zyq 15-10-8
 */
public class Util_LOGFile {

	private static Util_LOGFile instance;
	private String mLOGFilePath;
	private Writer mWriter;
	private SimpleDateFormat df;

	public static Util_LOGFile getInstance() {
		if (instance == null) {
			instance = new Util_LOGFile();
		}
		return instance;
	}

	/**
	 * 打开文件
	 *
	 * @param filePath
	 */
	public void open(String filePath) {
		if (filePath == null && TextUtils.isEmpty(filePath)) {
			return;
		}
		this.mLOGFilePath = filePath;
		File file = new File(filePath);
		try {
			if (file.exists() && file.isDirectory()) {
				return;
			} else if (!file.exists()) {
				file.createNewFile();
			}
			mWriter = new BufferedWriter(new FileWriter(file, false), 2048);
			df = new SimpleDateFormat("[yy-MM-dd hh:mm:ss]: ");
		} catch (IOException e) {
			Log.e("logFile",e.toString());
		}
	}

	public void write(String log) {
		try {
			if (mLOGFilePath == null)
				throw new IllegalArgumentException("please open file before");
			if (mWriter == null) return;

			mWriter.write(df.format(new Date()));
			mWriter.write(log);
			mWriter.write("\n");
			mWriter.flush();
		}catch (IOException e){
            Log.e("logFile",e.toString());
		}
	}


}
