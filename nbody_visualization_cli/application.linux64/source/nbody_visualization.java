import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import java.util.Properties; 
import java.io.Reader; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class nbody_visualization extends PApplet {




public String readInputFilePath() {
  return args[0];
}

BufferedReader reader;
String line;
int steps = 0; 
String path = "";
float scale = 0.5f;

public void setup() {
  
  path = readInputFilePath();
  // Open the file from the createWriter() example
  reader = createReader(path);
  
  
  strokeWeight(1);
  stroke(255, 100);
  
  background(0);
     
    //background(0);
    frameRate(60);
}

public void draw() {
  background(0);
   try {       
    line = reader.readLine();
    int year = steps / 365;
    int day = steps % 365;
    text("Year: " + year + " Day: " + day + "\nSimulation Speed: " + round(frameRate) + " days/sec", 10, 20);
  } catch (IOException e) {
    e.printStackTrace();
    line = null;
  }
  if (line == null) {
    // Stop reading because of an error or file is empty
    delay(10000);
    noLoop();  
  } else {
    
      String[] pieces = split(line, TAB);
      int numBodies = pieces.length / 3;
      text("Bodies: " + numBodies, 10, 50);
      for(int i = 0; i < numBodies; i++) {
        // each step is a day
        float x = PApplet.parseFloat(pieces[3*i+0]) * scale * (width /2) + (width/2) ;
        float y = PApplet.parseFloat(pieces[3*i+1]) * scale * (height/2) + (height/2);
   
        fill(255,255,255);
        stroke(255,255,255);
        strokeWeight(3);
        point(x,y);    
  
      }
    
  }
  steps++;
}

  public void settings() {  size(800,800);  smooth(); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "nbody_visualization" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
