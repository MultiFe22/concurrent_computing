import java.util.Random;

class Monitor{
  private int sensorQueue, actuatorOn, sensorOn, nextOverwrite;
  public Data[] data = new Data[60];
  public Monitor(){
    this.sensorQueue = 0;
    this.actuatorOn = 0;
    this.actuatorOn = 0;
    this.nextOverwrite = 0;
    System.out.println("Monitor start");
    for(int i = 0; i < 60; i++){
      this.data[i] = new Data();
    }
  }

  public synchronized int getNextOverwrite(){
    this.nextOverwrite++;
    this.nextOverwrite = this.nextOverwrite%60;
    return this.nextOverwrite;
  }

  public synchronized void sensorIn(){
    try{
      this.sensorQueue++;
      //System.out.println("Queue: " + this.sensorQueue);
      while(this.actuatorOn > 0 || this.sensorOn > 0) wait();
      this.sensorQueue--;
      this.sensorOn++;
      //System.out.println("On: " + this.sensorOn);
    } catch (InterruptedException e) { }
  }

  public synchronized void actuatorIn(){
    try{
      while(this.sensorQueue > 0 || this.sensorOn > 0){
        wait();
      }
      this.actuatorOn++;
    } catch (InterruptedException e) { }
  }

  public synchronized void sensorOut(){
    this.sensorOn--;
    notifyAll();
  }

  public synchronized void actuatorOut(){
    this.actuatorOn--;
    notifyAll();

  }

  
}

class Data{
  private int temperature;
  private int sensorId;
  private int id;
  public Data(){
    
  }

  public void input(int sensorId, int temperature, int id){
    this.id = id;
    this.temperature = temperature;
    this.sensorId = sensorId;
  }

  public int getTemperature(){
    return this.temperature;
  }

  public int getId(){
    return this.sensorId;
  }
}

class Sensor extends Thread {
  private int id;
  private int delay;
  private Monitor monitor;
  private int readCounter;
  
  public Sensor(int id, Monitor monitor){
    this.id = id;
    this.delay = 1000;
    this.monitor = monitor;
    this.readCounter = 0;
  }
  

  private int read(){
    Random readValue = new Random();
    int n = readValue.nextInt(16);
    n += 26;
    return n;
  }
  
  private void updateReadCounter(){
    this.readCounter++;
  }

  public void run(){
    try{
      while(true){
        this.monitor.sensorIn();
        int temperature = this.read();
        System.out.println("Leu do " +this.id);
        if(temperature > 30){
          this.updateReadCounter();
          int i = this.monitor.getNextOverwrite();
          /*System.out.println("index " + i);
          System.out.println("id " + this.id);
          System.out.println("temp " + temperature);*/
          this.monitor.data[i].input(this.id, temperature, this.readCounter);
        }
        this.monitor.sensorOut();
        sleep(this.delay);
      }
    } catch (InterruptedException e) { }
  } 

}

class Actuator extends Thread {
  private int id;
  private int delay;
  private Monitor monitor;
  public Actuator(int id, Monitor monitor){
    this.delay = 2000;
    this.id = id;
    this.monitor = monitor;
  }

  private void redSign(){ System.out.println("RED ALERT"); }

  private void yellowSign(){ System.out.println("YELLOW ALERT"); }

  private void normalSign(){ System.out.println("NORMAL ALERT"); }


  public void run(){
    try{
      while(true){
        this.monitor.actuatorIn();
        System.out.println("Atuador");
        int count = 0;
        int sequence = 0;
        int signal = 0;
        float average = 0;
        for(int i = 59;i >= 0 ;i--){
          if (this.monitor.data[i].getId() == this.id){
            sequence++;
            average += this.monitor.data[i].getTemperature();
            if(this.monitor.data[i].getTemperature() > 35) {
              count++;
              if(count == sequence && sequence == 5) {
                this.redSign();
                signal++;
              }
              else if(count == 5 && sequence <= 15){
                this.yellowSign();
                signal++; 
              }            
            }

          }
          if( (i == 0) && (signal == 0) && (count > 0)) this.normalSign();
          
          if(i == 0)System.out.println("Average temperature of sensor " + id + " = " + average/sequence);

        }
        this.monitor.actuatorOut();
        sleep(this.delay);
      }
    } catch (InterruptedException e) { }
  }




  
}

class Main {
  static final int sensorNum = 4;
  public static void main(String[] args) {
    Monitor monitor = new Monitor();
    Sensor[] sensors = new Sensor[sensorNum];
    Actuator[] actuators = new Actuator[sensorNum];
    for(int i = 0; i < sensorNum; i++){
      sensors[i] = new Sensor(i+1, monitor);
      actuators[i] = new Actuator(i+1, monitor);
      actuators[i].start();
      sensors[i].start();
      
    }
  }
}