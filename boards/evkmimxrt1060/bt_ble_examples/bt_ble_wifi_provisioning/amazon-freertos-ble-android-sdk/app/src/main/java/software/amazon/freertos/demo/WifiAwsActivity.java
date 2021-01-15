package software.amazon.freertos.demo;

import android.support.v4.app.Fragment;

public class WifiAwsActivity extends SingleFragmentActivity {
    @Override
    protected Fragment createFragment() {
        return new WifiAwsFragment();
    }
}
