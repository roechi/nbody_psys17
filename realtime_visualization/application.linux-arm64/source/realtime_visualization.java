import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class realtime_visualization extends PApplet {

BufferedReader reader;
String line;
int steps = 0;
float scale = 0.1f;
float shiftHorizontal = 0.0f;
float shiftVertical = 0.0f;
int userFrameRate = 60;
boolean autoScale = false;
float verticalDim = 0.0f;
float horizontalDim = 0.0f;

public void setup() {
  
  background(0);  
  
  frameRate(userFrameRate);
  reader = createReader(args[0]);
}

public void draw() {
  clear();
  background(0);
  
  try {
    line = reader.readLine();
  } catch (IOException e) {
    e.printStackTrace();
    line = null;
  }
  
  if (line == null) {
    fill(255,255,255);
    stroke(255,255,255);
    strokeWeight(1);
    textSize(25);
    textAlign(CENTER,CENTER);
    text("Stream is empty.", width/2, height/2);
    //noLoop();  
  } else {
    String[] pieces = split(line, TAB);
    int numBodies = pieces.length / 3;
    int year = steps / 365;
    int day = steps % 365;
    fill(255,255,255);
    stroke(255,255,255);
    textSize(12);
    textAlign(LEFT,LEFT);
    
    if(autoScale) {
      text("Year: " + year + " Day: " + day +
     "\nSimulation Speed: " + round(frameRate) + " days/sec" + 
     "\nBodies: " + numBodies +
     "\nAuto-Scale: " + (autoScale ? "On" : "Off") +
     "\nDimension: " + nf(horizontalDim,2,1) + " x "  + nf(verticalDim,2,1)+ " AU", 10, 20);
      
      text("Interface:" +
     "\n[Toggle Auto-Scale] 'A'",width-200,20);
    } else {
      
      text("Year: " + year + " Day: " + day +
     "\nSimulation Speed: " + round(frameRate) + " days/sec" + 
     "\nBodies: " + numBodies +
     "\nAuto-Scale: " + (autoScale ? "On" : "Off"), 10, 20);
         
      text("Interface:" +
     "\n[Toggle Auto-Scale] 'A'" +
     "\n[Offset] '\u2190 \u2192 \u2191 \u2193'" +
     "\n[Scale] '+ -'" +
     "\n[Reset Offset/Scale] 'C'",width-200,20); 
    }

    
    float superScale = 0.0f;
    float min_x = MAX_FLOAT;
    float max_x = MIN_FLOAT;
    float min_y = MAX_FLOAT;
    float max_y = MIN_FLOAT;
    for(int i = 0; i < numBodies; i++) {
      float x = Float.valueOf(pieces[3*i+0]);
      float y = Float.valueOf(pieces[3*i+1]);
      if(x < min_x) {
        min_x = x;
      }
      if(x > max_x) {
        max_x = x;
      }
      if(y < min_y) {
        min_y = y;        
      }
      if(y > max_y) {
        max_y = y;
      }
    }
    superScale = max(max_x - min_x, max_y - min_y);
    verticalDim = max_y - min_y;
    horizontalDim = max_x - min_x;
    
    for(int i = 0; i < numBodies; i++) {
      float x = 0.0f;
      float y = 0.0f;
      if(autoScale) {
        x = width/2 + (Float.valueOf(pieces[3*i+0]) / superScale) * width;
        y = height/2 + (Float.valueOf(pieces[3*i+1]) / superScale) * height;  
      } else {
        x = PApplet.parseFloat(pieces[3*i+0]) * scale * (width /2) + (width/2) + shiftHorizontal;
        y = PApplet.parseFloat(pieces[3*i+1]) * scale * (height/2) + (height/2) + shiftVertical;
      }
      

      fill(0,255,0,128);
      stroke(0,255,0,128);
      strokeWeight(2);
      point(x,y);
    }
  }
  steps++;
}


public void keyPressed() {
   if (!autoScale && key == CODED){
     if (keyCode == UP){
       shiftVertical += 10;
     } else if (keyCode == DOWN) {
       shiftVertical -= 10;
     } else if (keyCode == LEFT) {
       shiftHorizontal += 10;
     } else if (keyCode == RIGHT) {
       shiftHorizontal -= 10;
     }
   } else if (!autoScale) {
     if (key == '+'){
       scale *= 1.1f;
     } else if (key == '-') {
       scale *= 0.9f;
     } else if (key == 'c') {
       shiftHorizontal = 0;
       shiftVertical = 0;
       scale = 0.15f;
     }
   }
 
  if(key == 'a') {
    autoScale = !autoScale; 
  }
  
}
  public void settings() {  size(1000,700);  smooth(); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "realtime_visualization" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
