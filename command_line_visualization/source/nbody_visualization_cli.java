import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import java.util.Properties; 
import java.io.Reader; 
import com.hamoid.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class nbody_visualization_cli extends PApplet {





VideoExport videoExport;
boolean recording = false;

public String readInputFilePath() {
  return args[0];
}

BufferedReader reader;
String line;
int steps = 0; 
String path = "";

float scale = 0.15f;
float shiftVertical = 0.0f;
float shiftHorizontal = 0.0f;
float bodySize = 5;
int bodyColor = color(0,255,0,128);

public void setup() {
  
  path = readInputFilePath();
  //path = "/Users/jaszkowic/Desktop/positions.txt";
  //path = "../resources/crazy_universe.txt";
   //Open the file from the createWriter() example
  reader = createReader(path);
  
  
  strokeWeight(1);
  stroke(255, 100);
  
  background(0);
  frameRate(60);
  
    videoExport = new VideoExport(this);
  videoExport.setFrameRate(60);  
}

public void draw() {
  background(0);
   try {       
    line = reader.readLine();
  } catch (IOException e) {
    e.printStackTrace();
    line = null;
  }
  
  if (line == null) {
    // Stop reading because of an error or file is empty
    delay(10000);
    noLoop();  
  } else {
    // Parse Bodies
    String[] pieces = split(line, TAB);
    
    // Render status
    int numBodies = pieces.length / 3;
    if(numBodies > 64) {
        bodySize = 2;
        bodyColor = color(0,255,0,128);
    } else {
        bodySize = 5;
        bodyColor = color(255,255,255);
    }
    int year = steps / 365;
    int day = steps % 365;
    stroke(255,255,255);
    fill(255,255,255);
    text("Year: " + year + " Day: " + day + "\nSimulation Speed: " + round(frameRate) + " days/sec\nBodies: " + numBodies + "\nRecording: " + recording, 10, 20);
    
    // Render bodies
    for(int i = 0; i < numBodies; i++) {
      float x = PApplet.parseFloat(pieces[3*i+0]) * scale * (width /2) + (width/2) + shiftHorizontal;
      float y = PApplet.parseFloat(pieces[3*i+1]) * scale * (height/2) + (height/2) + shiftVertical;
      fill(bodyColor);
      stroke(bodyColor);
      strokeWeight(bodySize);
      point(x,y);    
    }
    
  }
  
  // Record to file
  if(recording) {
   videoExport.saveFrame(); 
  }
  
  steps++;
}

public void keyPressed() {
 if (key == CODED){
   if (keyCode == UP){
     shiftVertical -= 10;
   } else if (keyCode == DOWN) {
     shiftVertical += 10;
   } else if (keyCode == LEFT) {
     shiftHorizontal += 10;
   } else if (keyCode == RIGHT) {
     shiftHorizontal -= 10;
   }
 } else {
   if (key == '+'){
     scale *= 1.1f;
   } else if (key == '-') {
     scale *= 0.9f;
   } else if (key == 'c') {
     shiftHorizontal = 0;
     shiftVertical = 0;
     scale = 0.15f;
   } else if (key == 'q') {
     recording = false;
      videoExport.endMovie();
   } else if (key == 's') {
      // Start exporting after adjusting the settings.
      recording = true;
      videoExport.startMovie();
   }
 }
}
  public void settings() {  size(1000,700);  smooth(); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "nbody_visualization_cli" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
