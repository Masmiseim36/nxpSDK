package software.amazon.freertos.demo;

import android.content.Context;
import android.content.Intent;
import android.support.v4.app.Fragment;

public class ViewAwsShadowActivity extends SingleFragmentActivity {
    public static final String PROFENCES_URI = null;
    @Override
    protected Fragment createFragment() {
        return new ViewAwsShadowFragment();
    }

    public static Intent newIntent(Context packageContext, String uri) {
        Intent intent = new Intent(packageContext, ViewAwsShadowActivity.class);
        intent.putExtra(PROFENCES_URI, uri);
        return intent;
    }
}
