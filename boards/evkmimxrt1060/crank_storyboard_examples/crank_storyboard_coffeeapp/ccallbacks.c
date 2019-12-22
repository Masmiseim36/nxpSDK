#include <stdlib.h>
#include <gre/sdk/sbresource_vfs.h>
#include <gre/gre.h>
#include <ccallbacks.h>

void setDynamicValue(gr_application_t *app, char *keyName, int value){
	int *data = NULL;
	char *format = NULL;

 
	data = calloc(1, sizeof(int)); 
 	if (gr_application_get_data(app, keyName, (void **) &data, &format) == -1) {
		return;
	}
    	*data = value;
	gr_application_set_data(app, keyName, format, (void *) data);
}

void cbResetSizes(gr_action_context_t *action_context) {
    int value; 
    gr_application_t *app; 

    app = gr_context_get_application(action_context); 
    if(app == NULL) {
	    return;
    }
    value = 0; 
    setDynamicValue(app, "size_layer.size_blocker.grd_hidden", value);
    setDynamicValue(app, "size_layer.SELECT_SIZE.alpha", value);
    setDynamicValue(app, "size_layer.cup_sm_control.alpha", value);
    setDynamicValue(app, "size_layer.cup_sm_control.alpha1", value);
    setDynamicValue(app, "size_layer.cup_sm_control.alpha2", value);
    setDynamicValue(app, "size_layer.cup_sm_control.grd_width", value);
    setDynamicValue(app, "size_layer.cup_sm_control.grd_height", value);
    setDynamicValue(app, "size_layer.cup_md_control.alpha", value);
    setDynamicValue(app, "size_layer.cup_md_control.alpha1", value);
    setDynamicValue(app, "size_layer.cup_md_control.alpha2", value);
    setDynamicValue(app, "size_layer.cup_md_control.grd_width", value);
    setDynamicValue(app, "size_layer.cup_md_control.grd_height", value); 
    setDynamicValue(app, "size_layer.cup_lg_control.alpha", value);
    setDynamicValue(app, "size_layer.cup_lg_control.alpha1", value);
    setDynamicValue(app, "size_layer.cup_lg_control.alpha2", value);
    setDynamicValue(app, "size_layer.cup_lg_control.grd_width", value);
    setDynamicValue(app, "size_layer.cup_lg_control.grd_height", value); 

    value = 255;
    setDynamicValue(app, "size_layer.cup_sm_control.alpha3", value);
    setDynamicValue(app, "size_layer.cup_md_control.alpha3", value);
    setDynamicValue(app, "size_layer.cup_lg_control.alpha3", value);
  
    value = 178;
    setDynamicValue(app, "size_layer.cup_sm_control.alpha4", value);
    setDynamicValue(app, "size_layer.cup_md_control.alpha4", value);
    setDynamicValue(app, "size_layer.cup_lg_control.alpha4", value);
  
    value = 90; 
    setDynamicValue(app, "size_layer.cup_sm_control.grd_x", value);

    value = 153; 
    setDynamicValue(app, "size_layer.cup_sm_control.grd_y", value);
    setDynamicValue(app, "size_layer.cup_md_control.grd_y", value);
    setDynamicValue(app, "size_layer.cup_lg_control.grd_y", value);

    value = 240; 
    setDynamicValue(app, "size_layer.cup_md_control.grd_x", value);

    value = 390; 
    setDynamicValue(app, "size_layer.cup_lg_control.grd_x", value);

    value = 230;
    setDynamicValue(app, "logo_overlay.logo.grd_y", value);
}

void cbResetStart(gr_action_context_t *action_context) {
    int value; 
    gr_application_t *app;

    app = gr_context_get_application(action_context);
    if(app == NULL) {
            return;
    }

    value = 1;
    setDynamicValue(app, "start_layer.start_blocker.grd_hidden", value);
    value = 0; 
    setDynamicValue(app, "size_layer.cup_md_control.alpha", value);
    value = 153; 
    setDynamicValue(app, "size_layer.cup_md_control.grd_y", value);
    value = 204;
    setDynamicValue(app, "start_layer.START.grd_y", value);
    value = 189; 
    setDynamicValue(app, "start_layer.START_glow.grd_y", value);
    value = 12; 
    setDynamicValue(app, "start_layer.coffee.grd_y", value);
    value = 37; 
    setDynamicValue(app, "start_layer.start_circle.grd_y", value);
    value = 2;
    setDynamicValue(app, "start_layer.start_blur.grd_y", value);
    value = 53;
    setDynamicValue(app, "logo_overlay_home_screen.logo.grd_y", value);
}

void cbResetBrew(gr_action_context_t *action_context) {
    int value;
    gr_application_t *app;

    app = gr_context_get_application(action_context);
    if(app == NULL) {
            return;
    }


    value = 0; 
    setDynamicValue(app, "brew_layer.brew_blocker.grd_hidden", value);
    setDynamicValue(app, "brew_layer.brew_control.alpha", value);
    setDynamicValue(app, "brew_layer.brew_control.alpha1", value);
    setDynamicValue(app, "brew_layer.brew_control.alpha2", value);
    setDynamicValue(app, "brew_layer.brew_control.alpha3", value);
    setDynamicValue(app, "brew_layer.brew_control.alpha4", value);
    setDynamicValue(app, "brew_layer.direction_control.alpha", value);
    setDynamicValue(app, "brew_layer.direction_control.alpha1", value);
    setDynamicValue(app, "brew_layer.direction_control.alpha2", value);
    setDynamicValue(app, "brew_base_layer.bottom_bar.grd_height", value);
    
    value = 255; 
    setDynamicValue(app, "brew_layer.BREW_CUP.alpha", value);
    setDynamicValue(app, "brew_layer.cup_small.alpha", value);
    setDynamicValue(app, "brew_layer.cup_small.alpha1", value);
    setDynamicValue(app, "brew_layer.cup_small.alpha2", value);
    setDynamicValue(app, "brew_layer.cup_medium.alpha", value);
    setDynamicValue(app, "brew_layer.cup_medium.alpha1", value);
    setDynamicValue(app, "brew_layer.cup_medium.alpha2", value);
    setDynamicValue(app, "brew_layer.cup_large.alpha", value);
    setDynamicValue(app, "brew_layer.cup_large.alpha1", value);
    setDynamicValue(app, "brew_layer.cup_large.alpha2", value);
    setDynamicValue(app, "brew_layer.underline.alpha", value);
    
    value = -90; 
    setDynamicValue(app, "brew_layer.brew_control.endAngle", value);
    
    value = 57; 
    setDynamicValue(app, "brew_layer.brew_control.y", value);
    
    value = 272; 
    setDynamicValue(app, "brew_base_layer.bottom_bar.grd_y", value);
    
    value = 280;
    setDynamicValue(app, "brew_base_layer.back_control.grd_y", value);
    
    value = 203;
    setDynamicValue(app, "brew_base_layer.Crank.grd_y", value);
    
    value = 1;
    setDynamicValue(app, "brew_layer.cup_medium.grd_hidden", value);
    setDynamicValue(app, "brew_layer.cup_large.grd_hidden", value);
    setDynamicValue(app, "brew_layer.cup_small.grd_hidden", value);
    
    value = 200;
    setDynamicValue(app, "brew_layer.cupCopy.x", value);
    
    value = 327;
    setDynamicValue(app, "brew_layer.brew_control.grd_x", value);
}

void cbResetBrewing(gr_action_context_t *action_context) {
    int value; 
    gr_application_t *app;

    app = gr_context_get_application(action_context);
    if(app == NULL) {
            return;
    }

    
    value = 1; 
    setDynamicValue(app, "start_layer.start_blocker.grd_hidden", value);
    
    value = 0; 
    setDynamicValue(app, "size_layer.cup_md_control.alpha", value);
    setDynamicValue(app, "brewing_layer.glow.alpha", value);
    setDynamicValue(app, "brewing_layer.ready.alpha", value);
    setDynamicValue(app, "brewing_layer.Enjoy.alpha1", value);
    setDynamicValue(app, "brewing_layer.cup_fill_control.alpha", value);
    
    value = 153; 
    setDynamicValue(app, "size_layer.cup_md_control.grd_y", value);
    
    value = 1300; 
    setDynamicValue(app, "brewing_layer.ready.y", value);
    
    value = 255; 
    setDynamicValue(app, "brewing_layer.Enjoy.alpha", value);
    setDynamicValue(app, "brewing_layer.brewing_control.alpha", value);
    setDynamicValue(app, "brewing_layer.brewing_control.alpha1", value);
    setDynamicValue(app, "brewing_layer.cup.alpha", value);
    setDynamicValue(app, "brewing_layer.cup_fill_control.alpha1", value);
    setDynamicValue(app, "brewing_layer.cup_fill_control.alpha2", value);
    setDynamicValue(app, "brewing_layer.cup_fill_control.alpha3", value);
    
    value = 64; 
    setDynamicValue(app, "brewing_layer.brewing_control.alpha2", value);
    
    value = 270; 
    setDynamicValue(app, "brewing_layer.brewing_control.endAngle", value);
    
    value = -44; 
    setDynamicValue(app, "brewing_layer.cup_fill_control.x", value);
    
    value = 53; 
    setDynamicValue(app, "brewing_layer.cup_fill_control.x1", value);
    
    value = 131; 
    setDynamicValue(app, "brewing_layer.cup_fill_control.x2", value);
    
    value = 83; 
    setDynamicValue(app, "brewing_layer.cup_fill_control.y", value);
    setDynamicValue(app, "brewing_layer.cup_fill_control.y1", value);
}
