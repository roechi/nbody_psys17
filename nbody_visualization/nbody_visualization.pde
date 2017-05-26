
BufferedReader reader;
String line;
int steps = 0; 
String path = "/Users/jaszkowic/Desktop/positions.txt";

void setup() {
  size(700,700);
  strokeWeight(1);
  stroke(255, 100);
  smooth();
  background(0);
  // Open the file from the createWriter() example
  reader = createReader(path);   
    //background(0);
    frameRate(60);
}

void draw() {
  background(0);
   try {
    line = reader.readLine();
    int year = steps / 365;
    int day = steps % 365;
    text("Year: " + year + " Day: " + day + "\nSimulation Speed: " + round(frameRate) + " days/sec", 10, 20);
    //println(line);
  } catch (IOException e) {
    e.printStackTrace();
    line = null;
  }
  if (line == null) {
    // Stop reading because of an error or file is empty
    delay(5000);
    noLoop();  
  } else {
    String[] pieces = split(line, TAB);
    int numBodies = pieces.length / 3;
    for(int i = 0; i < numBodies; i++) {
      // each step is a day
      float x = float(pieces[3*i+0]) * (width /2) + (width/2);
      float y = float(pieces[3*i+1]) * (height/2) + (height/2);
 
      fill(255,255,255);
      stroke(255,255,255);
      strokeWeight(3);
      point(x,y);    

    }
  }
  steps++;
}

void mousePressed() {
  // restart
  line = null;
  reader = createReader(path);
  steps = 0;
}