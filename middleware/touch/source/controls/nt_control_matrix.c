/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "../source/system/nt_system_prv.h"
#include "../source/system/nt_system_mem_prv.h"
#include "nt_controls.h"
#include "../source/controls/nt_controls_prv.h"
#include "nt_control_matrix.h"
#include "nt_control_matrix_prv.h"
#include "../source/filters/nt_filters_prv.h"

/* Invoke callback based on the even has occurred */
static void _nt_control_matrix_invoke_callback(struct nt_control_data *control,
                                      enum nt_control_matrix_event event,
                                      nt_coordinates8_xy *points,
                                      uint32_t index_or_gesture);
/* The Matrix control initialization function. */
static int32_t _nt_control_matrix_init(struct nt_control_data *control);
/* The Matrix control process function. */
static int32_t _nt_control_matrix_process(struct nt_control_data *control);
/* The Matrix control function for position recognition */
void _nt_control_matrix_get_position (uint8_t *position1,
                                      uint8_t *position2,
                                      uint32_t state,
                                      uint8_t *add_val);
/* Function to calculate the coincidence amount to determine how much 
   was actual unlock gesture like stored one. */
uint16_t _nt_control_matrix_gesture_recognition_square(nt_coordinates64_xy *buffer,
                                                       nt_coordinates64_xy *gesture, uint16_t finger);
/* Function for swipe control gesture recognition */
uint16_t _nt_control_matrix_horiz_vertic_recognition(nt_coordinates64_xy *buffer, uint16_t finger);
/* Function for zoom control gesture recognition */
enum nt_control_matrix_control_gestures _nt_control_matrix_zoom_recognition(nt_coordinates64_xy *buffer);

/* The Matrix control interface. */
const struct nt_control_interface nt_control_matrix_interface = {
  .init        = _nt_control_matrix_init,
  .process     = _nt_control_matrix_process,
  .name        = NT_CONTROL_MATRIX_NAME,
  .params_size = sizeof(struct nt_control_matrix),
};

/* Clearing function */
void _nt_control_matrix_clean (nt_points64 *buffer);
/* Copy function for 16 bit points structure */
void _nt_control_matrix_copy (nt_points8 *buffer_copy, nt_points8 *buffer_orig);
/* Copy function for 64 bit points structure */
void _nt_control_matrix_copy_64 (nt_points64 *buffer_copy, nt_points64 *buffer_orig);

void nt_control_matrix_register_callback(const struct nt_control *control,
                                       nt_control_matrix_callback callback)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_matrix_interface);
    
    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);
    
    struct nt_control_matrix_data *ram = control_data->data.matrix;
    NT_ASSERT(ram != NULL);
    
    ram->callback = callback;
}


static void _nt_control_matrix_invoke_callback(struct nt_control_data *control,
                                      enum nt_control_matrix_event event,
                                      nt_coordinates8_xy *points,
                                      uint32_t index_or_gesture)
{
    struct nt_control_matrix_data *ram = control->data.matrix;
    NT_ASSERT(ram != NULL);
    
    if (ram->callback != NULL) 
    {
        ram->callback(control->rom, event, points, index_or_gesture);
    }
}  

int32_t nt_control_matrix_set_gesture(const struct nt_control *control, uint8_t gesture)
{   NT_ASSERT(control != NULL);
    NT_ASSERT(control->interface == &nt_control_matrix_interface);
    
    struct nt_control_data *control_data = _nt_control_get_data(control);
    NT_ASSERT(control_data != NULL);  
  
    struct nt_control_matrix_data *ram = control_data->data.matrix;
    NT_ASSERT(ram != NULL);
    
    if (gesture < NT_MATRIX_UNLOCK_GESTURES) /* gesture number can not be higher then size of gesture array */
    {   ram->select_gesture = gesture+1;     /* select_gesture must be higher then zero to store new gesture or replace old one */ 
        if (gesture >= ram->gesture_number)  /* check if real stored number of gesture is lover or equal to required store gesture */
            ram->gesture_number = gesture+1; /* if not then new gesture will be created */
        return NT_SUCCESS;
    }
    return NT_FAILURE;    
}

/* Position recognition for specific axis and one or two finger positions.
   In case of two fingers on the touch pad it can swap their coordinates, 
   because it groups the low and high coordinates together.
   This function eliminates the nearest electrode activations and calculates middle
   position only - reduces the finger touch size.
   If increase_resolution mode selected the position recognition uses enhanced algorithm with different adding constant 
   defined in initialization function. It enhances the position resolution by accepting the two-neighbour electrode as 
   a new virtual electrode in the middle.
*/ 
void _nt_control_matrix_get_position(uint8_t *position1,
                                     uint8_t *position2,
                                     uint32_t state,
                                     uint8_t *add_val) 
 
{   uint8_t actual_position[2] = {0,0};   
    uint8_t position = 1;
    uint8_t finger = 0;
    
    while(state)           
    {                                                   
        if (finger > 1)                                 /* maximum two finger are detected */
            finger = 1;                                 /* the next finger would be rewritten */
        
        uint32_t state_cut = state & 0x0007; 
        switch (state_cut)                              /* three nearest electrodes are touched simultaneously */ 
        {                 
        case 0x0007:                                    
            {   actual_position[finger++] = position + add_val[0]; /* use position in the middle */ 
                state >>= 3;                            /* update electrode status for next step */
                position += add_val[1];                 /* update position counter for next step */ 
            }                                           
            break;  
        case 0x0003:                                    /* two nearest electrodes are touched simultaneously */
            {   actual_position[finger++] = position + add_val[2];   /* use lower position */    
                state >>= 2;                            /* update electrode status for next step */
                position += add_val[3];                 /* update position counter for next step */
            }                                           
            break;   
        case 0x0005:                                    
            {   actual_position[finger++] = position;   /* use position in the middle */ 
                state >>= 1;                            /* update electrode status for next step */
                position += add_val[4];                 /* update position counter for next step */ 
            }                                           
            break;             
        case 0x0001:                                    /* no nearest electrodes touched together */
            {   actual_position[finger++] = position;   /* use exact position */ 
                state >>= 1;                            /* update electrode status for next step */
                position += add_val[4];                 /* update position counter for next step */
            }                                           
            break;                                      
        default:                                        /* no electrode touched in current step */
            {   state >>= 1;                            /* update electrode status for next step */
                position += add_val[5];                 /* update position counter for next step */
            }    
            break;
        }            
    }          
    *position1 = actual_position[0];
    *position2 = actual_position[1];               
}

/* Function to calculate the coincidence amount to determine how much 
   was actual unlock gesture like stored one. Sum of point to point distances squared. */
uint16_t _nt_control_matrix_gesture_recognition_square(nt_coordinates64_xy *buffer,
                                                       nt_coordinates64_xy *gesture, uint16_t finger)
{      
    uint64_t temp64;
    uint64_t temp_buffer ;
    uint64_t temp_gesture;
    int16_t temp16;
    uint16_t temp_return = 0;
    uint16_t n = 0;
    uint16_t size = 0;
         
    temp_buffer  = buffer ->x;
    temp_gesture = gesture->x;    
     
    if (temp_buffer > 0xffff)        
    {   
        for (n=0; n<2; n++)
        {   temp16 = 0;         
            if (temp_buffer != 0)      /* calculate clb for captured unlock gesture only if not zero */
            {   temp64 = temp_buffer;  
                while (!(temp64 & 0xf000000000000000))
                {   temp64 <<= 4;
                    temp16++;
                }
            }
            else
            {   temp16 += 16;
            }
            if (temp_gesture != 0)     /* calculate clb difference between capture and stored gesture only if not zero */
            {   temp64 = temp_gesture; 
                while (!(temp64 & 0xf000000000000000))
                {    temp64 <<= 4;
                    temp16--;
                }
            }
            else
            {   temp16 += 16;
            }
            if (temp16 < 0)            /* absolute value of clf difference */
                temp16 = -temp16;
            if (temp16 > 31)           /* difference overflow */
                temp16 = 0;
            if (temp16 > 2)            /* difference is too high */
                return 0x7fff;
            
            temp64 = temp_buffer & temp_gesture; 
            while (temp64)
            {   temp16 = (temp_buffer  & (uint64_t)0xf) -
                         (temp_gesture & (uint64_t)0xf);
                temp16 *= temp16;
                temp_return += (uint16_t)temp16;
                temp_buffer  >>= 4;
                temp_gesture >>= 4;
                temp64 >>= 4; 
                size++;
            }
            temp_buffer  = buffer ->y;
            temp_gesture = gesture->y;
                 
        }
        temp_return<<=4; /* multiplication by const to obtain useful result */
        size *= size;    /* div by second power to normalize the number - remove depending of unlock gesture size */
        return (temp_return/size);
    }
    else 
    {   return 0x7fff;
    }
}

/* Function recognizes weather drawn swipe is more enough horizontal or vertical. The conditions to recognized swipe gestures:
     - buffered gesture must be longer than three recognized positions horizontally or vertically (for one finger)
     - recognized positions horizontally or vertically and the movement in dominant direction must cover twice the amount 
       recognized positions than in insignificant direction. */ 
uint16_t _nt_control_matrix_horiz_vertic_recognition(nt_coordinates64_xy *buffer, uint16_t finger)
{      
    uint64_t temp64;  
    uint16_t temp16; 
    int8_t sum_buff_x [16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; /* array initialization */
    int8_t sum_buff_y [16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; /* array initialization */
    int8_t sum_x = 0;
    int8_t sum_y = 0;
    
    if ((buffer->x | buffer->y) > 0xfff)      /* too short gesture - shorter than 3 point changes will not be recognized */       
    {          
        temp64  = buffer->x;
        while (temp64)                        /* Calculation of captured points lie in each column for captured gesture */
        {   temp16 = (temp64 & (uint64_t)0xf);/* Separate position from the first stored */
            sum_buff_x[temp16]++;             /* Find out the number of unchanged positions */
            temp64 >>= 4;                     /* Move to next position change in buffer */
        } 
        temp64 = 16;
        while (temp64--)                      /* Activated position calculation for x coordinate */
        {   if (sum_buff_x[temp64])
            sum_x++;              
        } 
        
        temp64  = buffer->y;
        while (temp64)                        /* Calculation of captured points lie in each row for captured gesture */
        {   temp16 = (temp64 & (uint64_t)0xf);/* Separate position from the first stored */
            sum_buff_y[temp16]++;             /* Find out the number of unchanged positions */
            temp64 >>= 4;                     /* Move to next position change in buffer */
        } 
        temp64 = 16;
        while (temp64--)                       /* Activated position calculation for y coordinate */
        {   if (sum_buff_y[temp64])
            sum_y++;              
        } 
        
        if (sum_x > sum_y)                     /* Dominant direction determination */
        {   sum_y <<= 1; 
            if (sum_x >= sum_y)                /* Projection to x axis is double y axis - gesture is enough horiz/vertic */ 
                return 0;                  
        }
        else 
        {   sum_x <<= 1; 
            if (sum_y >= sum_x)                /* Projection to y axis is double x axis - gesture is enough horiz/vertic */ 
                return 0;  
        }        
    }                                          
    return 0x7fff;                             /* return max error if gesture is too short or is not enough horiz/vertic */
}

/*  Function recognizes zoom in or zoom out gesture from buffered finger positions. It returns:
     - enum NT_MATRIX_ZOOM_IN  ... zoom in gesture detected,
     - enum NT_MATRIX_ZOOM_OUT ... zoom out gesture detected,
     - enum NT_MATRIX_NO_GESTURE ... no zoom gesture in buffer.
    The conditions to recognized zoom in/out gestures: 
     - buffered gesture must be longer than three recognized positions horizontally or vertically (for both fingers)
     - square of the position distance change must be higher(lower) then 15(-15) to increment zoom in(out) counter
     - the number of zoom in(out) counters must be higher then 1 and the ration between zoom in and zoom out counters
       must be double at least */
enum nt_control_matrix_control_gestures _nt_control_matrix_zoom_recognition(nt_coordinates64_xy *buffer)
{ 
    uint64_t p_1x = buffer[0].x;
    uint64_t p_1y = buffer[0].y; 
    uint64_t p_2x = buffer[1].x;     
    uint64_t p_2y = buffer[1].y;
  
    uint8_t square_dist;
    uint8_t square_dist_k_1 = 0;
    uint8_t zoom_in  = 0;
    uint8_t zoom_out = 0;
    
    if ((buffer->x | buffer->y) > 0xfff)                  /* Skip the zoom recognition if gesture is too short */ 
    {   while( (p_1x | p_1y) && (p_2x | p_2y))            /* Fingers distance calculation for each position change */
        {   int16_t temp = ((p_2x & 0xf) - (p_1x & 0xf)); /* X coordinate distance calculation */ 
            temp *= temp;                                 /* X distance squared */
            square_dist = temp;                           /* Store the X distance squared */
                                                          
            temp = ((p_2y & 0xf) - (p_1y & 0xf));         /* Y coordinate distance calculation */ 
            temp *= temp;                                 /* Y distance squared power of two */
            square_dist += temp;                          /* Store points distance */
                                                          /* Store the point to point distance squared */
            if (p_1x != buffer->x)                        /* Skip the first step, no previous points distance */
            {   temp = (uint16_t)square_dist - (uint16_t)square_dist_k_1; /* The change of distance calculation */
                if (temp > 15)                            /* Points distance increased  */
                    zoom_in++;                            /* Increase zoom in counter */
                else if (temp < -15)                      /* Points distance decreased */
                    zoom_out++;                           /* Increase zoom out counter */
            }           
            square_dist_k_1 = square_dist;                /* Store previous position change for future step */ 
            p_2x >>= 4;               /* Shift the buffered X coordinate for next captured position calculation */
            p_2y >>= 4;               /* Shift the buffered Y coordinate for next captured position calculation */
            p_1x >>= 4;               /* Shift the buffered X coordinate for next captured position calculation */
            p_1y >>= 4;               /* Shift the buffered Y coordinate for next captured position calculation */       
        }
        if (zoom_in > 1 && zoom_in >= zoom_out << 1)       /* Check the condition for zoom in control gesture recognition */
            return NT_MATRIX_ZOOM_IN;                      /* Zoom in gesture detected */         
        else if (zoom_out > 1 && zoom_out >= zoom_in << 1) /* Check the condition for zoom out control gesture recognition */
            return NT_MATRIX_ZOOM_OUT;                     /* Zoom out gesture detected */          
    }        
    return NT_MATRIX_NO_GESTURE; /* No zoom gesture detected */   
}

/* Matrix initialization function */
static int32_t _nt_control_matrix_init(struct nt_control_data *control)
{
    NT_ASSERT(control != NULL);
    NT_ASSERT(control->rom->interface == &nt_control_matrix_interface);
    NT_ASSERT(control->rom->control_params.matrix != NULL);
    NT_ASSERT(control->rom->control_params.matrix->gesture != NULL);    
    
    uint8_t n, pins;
    struct nt_control_matrix *rom = (struct nt_control_matrix*)control->rom->control_params.matrix;
    
    if (rom->touchpad_size_horizontal > 15U ||
        rom->touchpad_size_horizontal == 0U ||        
        rom->touchpad_size_vertical > 15 ||
        rom->touchpad_size_vertical == 0 ||
        rom->touchpad_size_vertical == 0U || 
        (rom->touchpad_size_horizontal > 8U && rom->increase_resolution > 0) ||   
        (rom->touchpad_size_vertical   > 8U && rom->increase_resolution > 0) ||   
        rom->dissimilarity_max > 500)                     
    {
      return NT_FAILURE;
    }
    
    control->data.matrix = _nt_mem_alloc(sizeof(struct nt_control_matrix_data));
    
    if(control->data.matrix == NULL)
    {
        return NT_OUT_OF_MEMORY;
    }
    
    if (_nt_control_check_data(control) != NT_SUCCESS) {
        return NT_FAILURE;
    }
    
    NT_ASSERT(control->data.matrix != NULL);
    struct nt_control_matrix_data *ram = control->data.matrix;

    ram->touchpad_mask_horizontal = 0;
    for (n=0 ; n < control->rom->control_params.matrix->touchpad_size_horizontal ; n++)  
        ram->touchpad_mask_horizontal |= (1 << n);

    /* Calculate touch pad mask */
    ram->touchpad_mask = 0;      
    if (rom->sensing_mode)
    {   /* mask for mutual-cap touch pad */
        pins = rom->touchpad_size_horizontal * rom->touchpad_size_vertical;  
        if (pins > 36) /* Maximum TSI mutual-cap channel restriction */     
            pins = 36;
    }
    else
    {   /* mask for self-cap touch pad */
        pins = rom->touchpad_size_horizontal + rom->touchpad_size_vertical;
        if (pins > 25) /* Maximum TSI self-cap channel restriction */     
            pins = 25;    
    }
   /* Set touch pad mask from touch pad sizes and touch pad sensing mode */         
    for (n=0 ; n < pins ; n++)  
        ram->touchpad_mask |= (1 << n);
    
    /* clear the quantities */
    ram->buffer_position         = 0; 
    _nt_control_matrix_clean(&ram->buffer);
    ram->double_tap_next_time    = 0;      
    ram->last_electode_states    = 0;   
    ram->select_gesture          = 0;
    ram->recognized_gesture      = 0;   
    ram->two_points_gesture_flag = 0;
    
    /* Evaluate how many gestures was defined */
    ram->gesture_number = 0;
    nt_points64 * gesture = rom->gesture;
    while (*(uint64_t*)gesture++ != 0)                    /* repeat to find empty gesture {{0,0},{0,0}}} */  
    {   /* Storing second finger movement */
        if (rom->gesture[ram->gesture_number][1].x || rom->gesture[ram->gesture_number][1].y) /* if second finger has data */
            ram->two_points_gesture_flag |= 0x1 << ram->gesture_number;     /* set two finger gesture flag */ 
        ram->gesture_number++;                                              /* increment gesture counter */
    } 
       
    /* Initialize constant for get_position function due to increase_resolution mode selected */    
    if (rom->increase_resolution)   
    {   ram->get_position_add_val[0] = 2;        
        ram->get_position_add_val[1] = 6;
        ram->get_position_add_val[2] = 1;
        ram->get_position_add_val[3] = 4;
        ram->get_position_add_val[4] = 2;
        ram->get_position_add_val[5] = 2;
    }
    else
    {   ram->get_position_add_val[0] = 1;    
        ram->get_position_add_val[1] = 3;
        ram->get_position_add_val[2] = 0;
        ram->get_position_add_val[3] = 2;
        ram->get_position_add_val[4] = 1;
        ram->get_position_add_val[5] = 1;        
    }
    return NT_SUCCESS;
}

/* Matrix process function */
static int32_t _nt_control_matrix_process(struct nt_control_data *control)
{
    NT_ASSERT(control != NULL); 
    NT_ASSERT(control->rom->interface == &nt_control_matrix_interface);
    
    struct nt_control_matrix_data *ram = control->data.matrix;
    /* Retype to unconst for gestures recognition from both ram and rom */
    struct nt_control_matrix *rom = (struct nt_control_matrix *)control->rom->control_params.matrix;

    NT_ASSERT(ram != NULL); 
    NT_ASSERT(rom != NULL);     
        
    if (!_nt_control_get_flag(control, NT_CONTROL_EN_FLAG)) 
    {   return NT_FAILURE; /* not enabled or data not ready */
    }
    
    if (_nt_control_get_flag(control, NT_CONTROL_NEW_DATA_FLAG))
    {   /* touch pad electrode state for matrix processing */ 
        uint64_t state = _nt_control_get_electrodes_state(control) & control->data.matrix->touchpad_mask;
        uint32_t state_x, state_y;
        
        /*** ONE OR MORE TOUCHPAD ELECTRODE CHANGED ***/
        /* Can detect position loss of the finger or one finger movement only,
        and recognize the release event.*/
        if (state ^ ram->last_electode_states)     
        {   /* Store last electrode states, x and y together. */
            ram->last_electode_states = state;
            
            /*** COVER ***/     
            if (state == (uint64_t)ram->touchpad_mask)  /* all touch pad electrode are active */
            {   _nt_control_set_flag(control, NT_MATRIX_GET_CONTROL_GESTURE_FLAG);
                _nt_control_matrix_invoke_callback(control, NT_MATRIX_GET_CONTROL_GESTURE,NULL, NT_MATRIX_COVER);                                   
                _nt_control_clear_flag(control, NT_CONTROL_NEW_DATA_FLAG); /* data processed */
                return NT_SUCCESS;
            }           
            
            /*** X / Y POSITION MUTUAL ***/
            if (rom->sensing_mode)
            {   /* x, y states for mutual-capacitance mode */
                uint16_t line_segment;
                uint16_t line;
                state_x = 0; 
                state_y = 0;  
                /* transfer state mutual value (each bit represents one mutual electrode) to x and y coordinates */ 
                /* line to line calculations */
                for (line = 0; line < rom->touchpad_size_vertical; line++)  
                {   /* cut current state to line segments by right shifts AND operation */
                    line_segment = (state >> (line << 2)) & control->data.matrix->touchpad_mask_horizontal; 
                    state_x |= line_segment;     /* store current state x values for each line */
                    if (line_segment >= 1)       /* store current state y values for line with pressed key */
                        state_y |= 1 << line;                     
                }
            }       
            else  /*** X / Y POSITION SELF-CAP ***/
            {   /* x, y states for self-capacitance mode */
                /* Separate each axix to own quantities for simple calculations */              
                state_x = state & ram->touchpad_mask_horizontal;
                state_y = state >> rom->touchpad_size_horizontal;
            }    /*** X / Y POSITION MUTUAL ***/        
            
            /*** TOUCH OR MOVEMENT ***/   
            /* At least one finger is touched or finger is moving. At least one position with x and y values is recognized.*/    
            if (state_x && state_y)
            {   /* Detect position of one or two finger in x and y coordinates. */
                _nt_control_matrix_get_position(&ram->actual_position[0].x, &ram->actual_position[1].x,
                                                state_x, &ram->get_position_add_val[0]);
                _nt_control_matrix_get_position(&ram->actual_position[0].y, &ram->actual_position[1].y,
                                                state_y, &ram->get_position_add_val[0]);               
                /* fill in not complete position for second finger if at lest one component is zero)*/
                if ((ram->actual_position[1].x != 0 ) && (ram->actual_position[1].y == 0))                    
                    ram->actual_position[1].y = ram->actual_position[0].y ;  /* copy y from first finger (shared one) */  
                else if ((ram->actual_position[1].x == 0) && (ram->actual_position[1].y != 0))
                    ram->actual_position[1].x = ram->actual_position[0].x ;  /* copy x from first finger (shared one) */             
                
                /*** TOUCH OR MOVEMENT - UNLOCK GESTURE BUFFERING INITIALIZATINON & DOUBLE TAB RECOGNITION ***/
                if (ram->buffer_position == 0)
                {   /* CONTROL GESTURE DELAY */
                    if (_nt_control_get_flag(control, NT_MATRIX_GET_CONTROL_GESTURE_FLAG))
                    {   /* Control gesture recognized, after push up hand after cover event the electrodes are changing states 
                           randomly therefore some delay is inserted before control flag clear and then matrix processes continues */       
                        uint32_t counter = nt_system_get_time_counter(); 
                        if (counter > ram->double_tap_next_time)  /* Get time counter */
                        {   _nt_control_clear_flag(control, NT_MATRIX_GET_CONTROL_GESTURE_FLAG); /* clear flag after delay */
                        }
                        _nt_control_clear_flag(control, NT_CONTROL_NEW_DATA_FLAG); /* data processed */
                        return NT_SUCCESS;                                         /* jump to avoid matrix processes */
                    }
                    _nt_control_clear_flag(control, NT_MATRIX_RELEASE_FLAG); 
                    _nt_control_set_flag  (control, NT_MATRIX_TOUCH_FLAG); 
                    _nt_control_matrix_clean(&ram->buffer);
                    
                    /*** TOUCH OR MOVEMENT - DOUBLE TAB RECOGNITION */ 
                    if (nt_system_get_time_counter() <= ram->double_tap_next_time)
                    {   /* Check if previous and current position are the same plus minus one position */                       
                        if ((_nt_abs_int32(ram->actual_position[0].x - ram->touch_position[0].x) <= 1) &&
                            (_nt_abs_int32(ram->actual_position[0].y - ram->touch_position[0].y) <= 1))  
                        {   /* delete second finger - only one finger double tab is recognized */                               
                            ram->touch_position[1].x = 0; 
                            ram->touch_position[1].y = 0;
                            _nt_control_set_flag(control, NT_MATRIX_GET_CONTROL_GESTURE_FLAG);
                            _nt_control_matrix_invoke_callback(control, NT_MATRIX_GET_CONTROL_GESTURE,
                                                               &ram->touch_position[0], NT_MATRIX_DOUBLE_TAP);                                        
                            _nt_control_clear_flag(control, NT_CONTROL_NEW_DATA_FLAG); /* data processed */
                            return NT_SUCCESS;
                        }
                    } 
                    else /*** TOUCH OR MOVEMENT - DOUBLE NOT RECOGNIZED */ 
                    {   _nt_control_matrix_copy (&ram->touch_position, &ram->actual_position);                       
                        _nt_control_matrix_invoke_callback(control, NT_MATRIX_TOUCH, 
                                                           &ram->touch_position[0], ram->recognized_gesture); 
                    }
                    
                    /* Store first position (touch position to buffer */
                    ram->buffer[0].x = ((uint64_t)ram->actual_position[0].x);
                    ram->buffer[0].y = ((uint64_t)ram->actual_position[0].y); 
                    ram->buffer[1].x = ((uint64_t)ram->actual_position[1].x);
                    ram->buffer[1].y = ((uint64_t)ram->actual_position[1].y);
                    ram->buffer_position += 4;
                } 
                else  /*** TOUCH OR MOVEMENT - UNLOCK GESTURE BUFFERING PROCESSES OR MOVEMENT ***/
                {   _nt_control_clear_flag(control, NT_MATRIX_TOUCH_FLAG);
                    _nt_control_clear_flag(control, NT_MATRIX_GET_UNLOCK_GESTURE_FLAG);
                    _nt_control_clear_flag(control, NT_MATRIX_SET_UNLOCK_GESTURE_FLAG);   
                    if (_nt_control_get_flag(control, NT_MATRIX_GET_CONTROL_GESTURE_FLAG))
                    {   ram->buffer_position = 0;
                        ram->double_tap_next_time = nt_system_get_time_counter() + rom->double_tap_interval;
                        _nt_control_clear_flag(control, NT_CONTROL_NEW_DATA_FLAG); /* data processed */
                        return NT_SUCCESS; 
                    }
                    else
                    {   _nt_control_set_flag  (control, NT_MATRIX_MOVEMENT_FLAG); 
                        _nt_control_matrix_invoke_callback(control, NT_MATRIX_MOVEMENT,
                                                           &ram->actual_position[0], ram->recognized_gesture);                    
                    }
                    if (ram->buffer_position < 64) /* Store the cached gesture which not exceed the buffer size */
                    { /* store recognized second and following positions to buffer */
                        uint8_t prev_position = ram->buffer_position - 4;
                        uint64_t is_changed = 0;
                        /* detect whether at least one coordinate is changed (position was changed) and not only near electrode was changed */  
                        for(uint8_t n = 0; n < 2; n++) 
					    {
                            is_changed |= (ram->buffer[n].x & (0xf << prev_position)) ^ (uint64_t)ram->actual_position[n].x << prev_position;
                            is_changed |= (ram->buffer[n].y & (0xf << prev_position)) ^ (uint64_t)ram->actual_position[n].y << prev_position;
                        }
                        if(is_changed) /* Store new position to buffer in case that position was changed */
					    {
                            ram->buffer[0].x |= ((uint64_t)ram->actual_position[0].x << ram->buffer_position);
                            ram->buffer[0].y |= ((uint64_t)ram->actual_position[0].y << ram->buffer_position); 
                            ram->buffer[1].x |= ((uint64_t)ram->actual_position[1].x << ram->buffer_position);
                            ram->buffer[1].y |= ((uint64_t)ram->actual_position[1].y << ram->buffer_position);                 
                            ram->buffer_position += 4;
                        }
                    }
                    else        /*** TOUCH OR MOVEMENT - UNLOCK GESTURE IS TOO LONG FOR BUFFER ***/               
                    {   _nt_control_matrix_clean(&ram->buffer); 
                        ram->buffer_position = 0;
                        
                        _nt_control_matrix_copy (&ram->release_position, &ram->actual_position);
                        
                        _nt_control_matrix_invoke_callback(control, NT_MATRIX_GESTURE_TOO_LONG,
                                                           &ram->release_position[0], ram->recognized_gesture);
                    } 
                }    /*** TOUCH OR MOVEMENT - UNLOCK GESTURE BUFFERING PROCESSES OR MOVEMENT ***/                  
            }   /*** TOUCH OR MOVEMENT ***/
            
            /*** END TOUCH OR MOVEMENT ***/ 
            /*** RELEASE ***/
            else if (!(state_x || state_y))
            {   uint16_t n;
                uint16_t temp;
                uint16_t temp1 = 0;
                 
                _nt_control_matrix_copy (&ram->release_position, &ram->actual_position);
                
                _nt_control_clear_flag(control, NT_MATRIX_MOVEMENT_FLAG);
                /* Set time for control gesture delay (avoid random electrode changing during hand pushing up after cover */
                ram->double_tap_next_time = nt_system_get_time_counter() +
                     rom->double_tap_interval; 
                
                if (_nt_control_get_flag(control, NT_MATRIX_GET_CONTROL_GESTURE_FLAG))
                {   ram->buffer_position = 0;  /*Init unlock gesture buffer when control gesture recognized */
                    _nt_control_clear_flag(control, NT_CONTROL_NEW_DATA_FLAG); /* data processed */
                    return NT_SUCCESS;                                         /* jump out gesture recognized */
                }
                else
                {   _nt_control_set_flag  (control, NT_MATRIX_RELEASE_FLAG); 
                }    
                
                /*** RELEASE - STORING UNLOCK GESTURE FROM BUFFER ***/
                /* Unlock gesture recording defined by selected gesture number, if zero value no record */
                if (ram->select_gesture > 0)
                {   /* Gesture for storing must be enough long for reliable recognition and the gesture size must not exceed buffer size */
                    if ((ram->buffer[0].x > 0xffff) && (ram->buffer[0].y > 0xffff) && (ram->buffer_position < 64))
                    {   /* Zero value means no gesture storing therefore gesture minus one */
                        uint16_t select_gesture = ram->select_gesture-1;
                        
                        ram->two_points_gesture_flag &= ~(0x1 << (select_gesture));  /* clear two finger gesture flag */                 
                        _nt_control_set_flag(control, NT_MATRIX_SET_UNLOCK_GESTURE_FLAG);   /* set set_gesture flag */ 
                   
                        /* store first and second x and y finger coordinates */                                                
                        _nt_control_matrix_copy_64(&rom->gesture[select_gesture], &ram->buffer);
                        
                        /* Storing second finger movement */
                        if (ram->buffer[1].x || ram->buffer[1].y)                    /* if second finger has data */
                        {   ram->two_points_gesture_flag |= 0x1 << (select_gesture); /* set two finger gesture flag */ 
                        } 
                        _nt_control_set_flag(control, NT_MATRIX_RELEASE_SET_GESTURE);
                        _nt_control_matrix_invoke_callback(control, NT_MATRIX_RELEASE_SET_GESTURE,
                                                           &ram->release_position[0], ram->select_gesture);
                        ram->select_gesture = 0;                                   /* clear gesture number value */
                    }
                    _nt_control_clear_flag(control, NT_CONTROL_NEW_DATA_FLAG); /* data processed */
                    return NT_SUCCESS;
                }
                
                /*** RELEASE - UNLOCK GESTURE RECOGNITION ***/
                /* Unlock gesture recognition algorithm */
                ram->recognized_gesture = 0;                            /* init by setting unknown gesture value */
                temp = rom->dissimilarity_max;           
                /* Compare buffered gesture with stored gestures, one by one */       
                for (n=0; n < ram->gesture_number; n++)  
                { 
                    ram->gesture_diff[n] = _nt_control_matrix_gesture_recognition_square(&ram->buffer[0],
                                                                                         &rom->gesture[n][0], 0);
                    /* In case of two point gestures compare the second finger movement as well */ 
                    if (ram->two_points_gesture_flag & (0x1 << n)) 
                    {   /* Adding dissimilarity for second finger */
                        ram->gesture_diff[n] += _nt_control_matrix_gesture_recognition_square(&ram->buffer[1], 
                                                                                              &rom->gesture[n][1], 1);                      
                        /* Make average of dissimilarities from first and second finger */
                        ram->gesture_diff[n] >>= 1; 
                    }                 
                    
                    /* Dissimilarities is under determine level - gesture recognized, if gesture is similar to 
                       more stored gestures the more similar gesture is used, avoid recognition after storing */
                    if ((ram->gesture_diff[n] < temp) && !_nt_control_get_flag(control, NT_MATRIX_RELEASE_SET_GESTURE)) 
                      {   temp = ram->gesture_diff[n];
                          ram->recognized_gesture = n + 1;
                      }
                      else 
                      {   if (ram->gesture_diff[n] == temp)   /* gestures with same dissimilarity */
                              ram->recognized_gesture = 0;    /* no gesture recognized */  
                      }
                }
                ram->buffer_position = 0; 

                /*** RELEASE - UNLOCK GESTURE NOT RECOGNIZED - SIMILAR GESTURES ***/
                /* Avoiding doubts when two or more gestures have similar dissimilarity */
                if (ram->recognized_gesture > 0)
                {   /* adding dissimilarity others to recognized gesture to assign new level*/ 
                    uint16_t lowDissCount = 0;       /* Counter for low dissimilarity */
                    for (temp1 = 0; temp1 < ram->gesture_number; temp1++)
                    {   /* new level is compared with all gesture */ 
                        if ((int32_t)ram->gesture_diff[temp1] < (int32_t)temp)                               
                            lowDissCount++;       /* incrementing counter when dissimilarity is under new level */      
                    }
                    if (lowDissCount > 1)         /* two gestures are under level (one is always - recognized gesture)*/                          
                    {   _nt_control_clear_flag(control, NT_MATRIX_GET_UNLOCK_GESTURE_FLAG); /* Clear get gesture flag */
                        ram->recognized_gesture = 0;   /* clean gesture information */ 
                    }
                    else 
                    {   _nt_control_set_flag(control, NT_MATRIX_GET_UNLOCK_GESTURE_FLAG);   /* Set get gesture flag */                 
                        /* Callback function with recognized or unrecognised parameter. */
                        _nt_control_matrix_invoke_callback(control, NT_MATRIX_RELEASE_GET_GESTURE,
                                                           &ram->release_position[0], ram->recognized_gesture);
                    }
                }
                else if (ram->select_gesture == 0)    /*** RELEASE - SWIPE GESTURE RECOGNITION - UNLOCK GESTURE NOT RECOGNIZED ***/
                {   /* SWIPE CONTROL GESTURES RECOGNITION IF ONE FINGER CAPTURED IN BUFFER */
                    if (ram->buffer[1].x == 0 && ram->buffer[1].y == 0)
                    {   /* Check if drawn track is enough horizontal or vertical for swipe gestures, skip after gesture stored*/                  
                        uint16_t swipe_diff = _nt_control_matrix_horiz_vertic_recognition(&ram->buffer[0], 0);
                        if (swipe_diff == 0 && !_nt_control_get_flag(control, NT_MATRIX_RELEASE_SET_GESTURE))
                        {   /* calculate the swipe direction */
                            int16_t dir_diff_x = (int16_t)ram->release_position[0].x - (int16_t)ram->touch_position[0].x;
                            int16_t dir_diff_y = (int16_t)ram->release_position[0].y - (int16_t)ram->touch_position[0].y;
                            _nt_control_set_flag(control, NT_MATRIX_GET_CONTROL_GESTURE_FLAG);
                            /* evaluate swipe type according to direction */
                            if (dir_diff_y < dir_diff_x && dir_diff_y > -dir_diff_x && dir_diff_x > 0)
                                _nt_control_matrix_invoke_callback(control, NT_MATRIX_GET_CONTROL_GESTURE,
                                                                   &ram->release_position[0], NT_MATRIX_SWIPE_RIGHT);                            
                            else if (dir_diff_y < -dir_diff_x && dir_diff_y > dir_diff_x && dir_diff_x < 0)                            
                                _nt_control_matrix_invoke_callback(control, NT_MATRIX_GET_CONTROL_GESTURE,
                                                                   &ram->release_position[0], NT_MATRIX_SWIPE_LEFT);      
                            else if (dir_diff_x < dir_diff_y && dir_diff_x > -dir_diff_y && dir_diff_y > 0)            
                                _nt_control_matrix_invoke_callback(control, NT_MATRIX_GET_CONTROL_GESTURE,
                                                                   &ram->release_position[0], NT_MATRIX_SWIPE_UP);  
                            else if (dir_diff_x < -dir_diff_y && dir_diff_x > dir_diff_y && dir_diff_y < 0) 
                                _nt_control_matrix_invoke_callback(control, NT_MATRIX_GET_CONTROL_GESTURE,
                                                                   &ram->release_position[0], NT_MATRIX_SWIPE_DOWN);    
                            _nt_control_matrix_clean(&ram->buffer); 
                        }                       
                        else    /*** RELEASE - NO GESTURE RECOGNIZED ***/
                        {   /* activate control gesture recognition after storing */
                            _nt_control_clear_flag(control, NT_MATRIX_RELEASE_SET_GESTURE);  
                            _nt_control_matrix_invoke_callback(control, NT_MATRIX_RELEASE,
                                                               &ram->release_position[0], ram->recognized_gesture); 
                        }
                    } /* SWIPE CONTROL GESTURES RECOGNITION IF ONE FINGER CAPTURED IN BUFFER */
                    else  /* ZOOM CONTROL GESTURES RECOGNITION IN CASE OF TWO FINGERS CAPTURED IN BUFFER */
                    {   enum nt_control_matrix_control_gestures zoom_gest = _nt_control_matrix_zoom_recognition(&ram->buffer[0]);
                        switch (zoom_gest)
                        {   case NT_MATRIX_ZOOM_IN:
                                _nt_control_matrix_invoke_callback(control, NT_MATRIX_GET_CONTROL_GESTURE,
                                                                   &ram->release_position[0], NT_MATRIX_ZOOM_IN); 
                                break;
                            case NT_MATRIX_ZOOM_OUT:     
                                _nt_control_matrix_invoke_callback(control, NT_MATRIX_GET_CONTROL_GESTURE,
                                                                   &ram->release_position[0], NT_MATRIX_ZOOM_OUT); 
                                break;
                            default:   
                                /* activate control gesture recognition after storing */
                                _nt_control_clear_flag(control, NT_MATRIX_RELEASE_SET_GESTURE);  
                                _nt_control_matrix_invoke_callback(control, NT_MATRIX_RELEASE,
                                                                   &ram->release_position[0], NT_MATRIX_NO_GESTURE); 
                                break;
                        }
                        _nt_control_matrix_clean(&ram->buffer); 
                    }   /* ZOOM CONTROL GESTURES RECOGNITION IN CASE OF TWO FINGERS CAPTURED IN BUFFER */  
                }   /*** RELEASE - SWIPE GESTURE RECOGNITION - UNLOCK GESTURE NOT RECOGNIZED ***/
            }  /*** RELEASE ***/   
			else  /*** UNKNOWN POSITION - TOUCH ON X OR Y ***/
			{   _nt_control_clear_flag(control, NT_MATRIX_RELEASE_FLAG);             /* Clear release flag */
			}
        }  /*** ONE OR MORE TOUCHPAD ELECTRODE CHANGED ***/   
    _nt_control_clear_flag(control, NT_CONTROL_NEW_DATA_FLAG); /* data processed */
    }
    return NT_SUCCESS;
}

void _nt_control_matrix_clean (nt_points64 *buffer)
{   uint64_t *buffer1 = (uint64_t*)buffer;
    uint8_t n=4;
    while(n--)
    {   
        *buffer1 = 0; 
        buffer1++;
    }
}

/* This function copies x and y coordinates of both fingers from second to first input pointer to nt_points8. */
void _nt_control_matrix_copy (nt_points8 *buffer_copy, nt_points8 *buffer_orig)
{   uint8_t *buffer_copy1 = (uint8_t*)buffer_copy;
    uint8_t *buffer_orig1 = (uint8_t*)buffer_orig;
    uint8_t n=4;
    while(n--)
    {   
        *buffer_copy1++ = *buffer_orig1++; 
    }
}

/* This function copies x and y coordinates of both fingers from second to first input pointer to nt_points64. */
void _nt_control_matrix_copy_64 (nt_points64 *buffer_copy, nt_points64 *buffer_orig)
{   uint64_t *buffer_copy1 = (uint64_t*)buffer_copy;
    uint64_t *buffer_orig1 = (uint64_t*)buffer_orig;
    uint8_t n=4;
    while(n--)
    {   
        *buffer_copy1++ = *buffer_orig1++; 
    }
}
