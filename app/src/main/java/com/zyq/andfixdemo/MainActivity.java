package com.zyq.andfixdemo;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.alipay.euler.andfix.Util_LOGFile;
import com.zyq.test.Fix;


public class MainActivity extends Activity {

	private static final String TFG = "euler";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.e(TFG, Fix.a("good"));
		Log.e(TFG, "" + new Fix().b("s1", "s2"));
		Toast.makeText(getApplicationContext(), "helloworld", Toast.LENGTH_LONG).show();
		Util_LOGFile.getInstance().write(TFG + "helloWorld");
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		android.os.Process.killProcess(android.os.Process.myPid());
	}

}
