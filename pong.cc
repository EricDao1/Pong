#include <allegro5/allegro_acodec.h>//allows .wav, .flac, .ogg, .it, .mod, .s3m, .xm
using namespace std;


//function to detect collision
//paramaters are x & y coordinates of top left corner of first box, then its width and height
//then same for other box
int bounding_box_collision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h);
 
//using constants in this game so you have sensible words, not unknown numbers in code
const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int BOUNCER_SIZE = 32;

//an enumeration is like a user-defined variable, with all the given values it can hold
enum MYKEYS {
   KEY_UP, KEY_DOWN
};
enum MYKEYS2 {
   KEY_W, KEY_S
};

 
int main(int argc, char **argv)
{
   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer = NULL;
   ALLEGRO_BITMAP *paddle = NULL;
   float paddle_x = 600;
   float paddle_y = 30;
   bool key[4] = { false, false, false, false };
   ALLEGRO_BITMAP *paddle2 = NULL;
   float paddle2_x = 20;
   float paddle2_y = 30;
   bool key2[4] = { false, false, false, false };
   bool redraw = true;
   bool doexit = false;
   int score1 = 0;
   int score2 = 0;
   float ball_dx = -5.5, ball_dy = 5.5;
   ALLEGRO_BITMAP *ball = NULL;
   int ball_x = 250;
   int ball_y = 250;
   


   //including some error messages to help debug
   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }
 
   if(!al_install_keyboard()) {
      fprintf(stderr, "failed to initialize the keyboard!\n");
      return -1;
   } 
 
   timer = al_create_timer(1.0 / FPS);
   if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
   }
 
   display = al_create_display(SCREEN_W, SCREEN_H);
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      al_destroy_timer(timer);
      return -1;
   }

   //hook up fonts for on-screen text
   al_init_font_addon(); 
   al_init_ttf_addon();
   ALLEGRO_FONT *font = al_load_ttf_font("Candara.ttf",72,0 );
 
   //audio stuff
   ALLEGRO_SAMPLE *sample=NULL;
   ALLEGRO_SAMPLE *sample2=NULL;
   ALLEGRO_SAMPLE *sample3=NULL;
   ALLEGRO_SAMPLE *sample4=NULL;
   al_install_audio();
 
   al_init_acodec_addon();
 
  al_reserve_samples(3);
   


  sample = al_load_sample( "Bleep.flac" );
  sample2 = al_load_sample("Bloop.wav");
  sample3 = al_load_sample("pallet.wav");
  sample4 = al_load_sample("sad.wav");
 
   al_rest(10.0);

   //set up the two squares
   paddle = al_create_bitmap(11, 50);
  
   paddle2 = al_create_bitmap(11, 50);

   ball = al_create_bitmap(25, 25);

   al_set_target_bitmap(ball);

   al_clear_to_color(al_map_rgb(864, 905, 755));
 
   al_set_target_bitmap(paddle);
 
   al_clear_to_color(al_map_rgb(589, 880, 364));

   al_set_target_bitmap(paddle2);
 
   al_clear_to_color(al_map_rgb(658, 5, 199));
 
   al_set_target_bitmap(al_get_backbuffer(display));

   
 
   //set up your event queue
   event_queue = al_create_event_queue();
   if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_bitmap(paddle);
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }
 
   al_register_event_source(event_queue, al_get_display_event_source(display));
 
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
 
   al_register_event_source(event_queue, al_get_keyboard_event_source());
 
   //draw the initial screen, start up the timer
   al_clear_to_color(al_map_rgb(0,0,0));
 
   al_flip_display();
 
   al_start_timer(timer);
 
   while(!doexit)
   { //al_play_sample(sample3, 1.0, 0.0,1,ALLEGRO_PLAYMODE_LOOP,NULL);
   
	   cout<<paddle_x <<" " <<paddle_y<< endl;
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
 
	  //if a clock ticks, check if we should be moving a square (by checking key or key2)
      if(ev.type == ALLEGRO_EVENT_TIMER) {

		
			  
			  if(key[KEY_UP] && paddle_y >= 4.0) {
            paddle_y -= 10.0;
         }
 
         if(key[KEY_DOWN] && paddle_y <= SCREEN_H - BOUNCER_SIZE - 4.0) {
            paddle_y += 10.0;
         }
 
   
		
		//////////////2 players 
		 //if(key2[KEY_W] && !(paddle_y < 0 )){
          //  paddle2_y -= 10.0;
         //}
 
        // if(key2[KEY_S] && !(paddle2_y > 480)) {
        //    paddle2_y += 10.0;
        // }




		 ////////////////1 player
		 if(paddle2_y > ball_y){
			paddle2_y = paddle2_y - 14.0;
		 }
		 else{
			 paddle2_y = paddle2_y + 14.0;
		 }

 
		 /////////////////////////ball movement!
		   if(ball_x < 0) {
			  ball_x = 300;
			  ball_y = 300;
			  score1++;//add points to right player

		  }
		   if(ball_x > 640 -32 ) {
			  ball_x = 250;
			  ball_y = 250;
			  score2++;//add points to left player
		   }

       
			//if the box hits the top wall OR the bottom wall
         if(ball_y < 0 || ball_y > 480 - 32) {
			 //flip the y direction
            ball_dy = -ball_dy;
         }
	
		 ball_x += ball_dx;
         ball_y += ball_dy;
		 
		 

         redraw = true;
      }
	  
	  //kill program if window "X" has been clicked
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         break;
      }

	  //update key/key2 if a key has been pressed
      else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
         switch(ev.keyboard.keycode) {

            case ALLEGRO_KEY_UP:
               key[KEY_UP] = true;
               break;
 
            case ALLEGRO_KEY_DOWN:
               key[KEY_DOWN] = true;
               break;
 
			   case ALLEGRO_KEY_W:
               key2[KEY_W] = true;
               break;
 
            case ALLEGRO_KEY_S:
               key2[KEY_S] = true;
               break;
 
           
			   
         }
      }
	  //update key/key2 if a key has been released
      else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
         switch(ev.keyboard.keycode) {

            case ALLEGRO_KEY_UP:
               key[KEY_UP] = false;
               break;
 
            case ALLEGRO_KEY_DOWN:
               key[KEY_DOWN] = false;
               break;
 
       

			   case ALLEGRO_KEY_W:
               key2[KEY_W] = false;
               break;
 
            case ALLEGRO_KEY_S:
               key2[KEY_S] = false;
               break;
 

			
         }
      }
	 

			//take all the above information and update screen
         al_clear_to_color(al_map_rgb(0,0,0));
 
         al_draw_bitmap(paddle, paddle_x, paddle_y, 0);
 
		 al_draw_bitmap(paddle2, paddle2_x, paddle2_y, 0);

		 al_draw_bitmap(ball, ball_x, ball_y, 0);
		 cout<< paddle_x<<" "<<paddle_y<<endl;

		 //call the bounding box function. if it returns 1, print out your collision message
		 if (bounding_box_collision(paddle2_x, paddle2_y, 11, 50, ball_x, ball_y, 25, 25)){
			  al_play_sample(sample2, 1.0, 0.0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
			ball_dx = -ball_dx+5;
			 ball_dy = -ball_dy+5;
		 }

		 //repeat above 4 lines for left paddle
		 if (bounding_box_collision(paddle_x, paddle_y, 11, 50, ball_x, ball_y, 25, 25)){
			  al_play_sample(sample2, 1.0, 0.0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
			ball_dx = -ball_dx+5;
			 ball_dy = -ball_dy+5;
		 }
         al_draw_textf(font, al_map_rgb(255,255,255), 450, 20,ALLEGRO_ALIGN_CENTRE,"%d", score1);
		 al_draw_textf(font, al_map_rgb(255,255,255), 150, 20,ALLEGRO_ALIGN_CENTRE,"%d", score2);


		 if(score1 == 20){
			  al_play_sample(sample, 1.0, 0.0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
			 al_draw_text(font, al_map_rgb(255,255,255), 450, 200,ALLEGRO_ALIGN_CENTRE, "WIN!!");
			 al_draw_text(font, al_map_rgb(255,255,255), 150, 200,ALLEGRO_ALIGN_CENTRE, "LOSE!");
			 ball_dx = 0;
			 ball_dy = 0;
			   
		 }
		 if(score2 == 20){
			  al_play_sample(sample4, 1.0, 0.0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
			 al_draw_text(font, al_map_rgb(255,255,255), 150, 200,ALLEGRO_ALIGN_CENTRE, "WIN!!");
			 al_draw_text(font, al_map_rgb(255,255,255), 450, 200, ALLEGRO_ALIGN_CENTRE, "LOSE!");
			 ball_dx = 0;
			 ball_dy = 0;
			
		 }

		 
		 
		 al_flip_display();
     
   }
 
   al_destroy_bitmap(paddle);
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
   al_destroy_sample(sample);
 
   return 0;
}

int bounding_box_collision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h)
{
    if ((b1_x > b2_x + b2_w - 1) || // is b1 on the right side of b2?
        (b1_y > b2_y + b2_h - 1) || // is b1 under b2?
        (b2_x > b1_x + b1_w - 1) || // is b2 on the right side of b1?
        (b2_y > b1_y + b1_h - 1))   // is b2 under b1?
    {
        // no collision
        return 0;
    }
 
    // collision
    return 1;
}


