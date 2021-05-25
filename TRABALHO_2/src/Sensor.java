import java.util.Random;

//This class models all the main methods of the sensor and creates all the required parameters

class Sensor extends Thread {
    private int id;
    private int delay;
    private Monitor monitor; //Share the same monitor with all the threads
    private int readCounter;

    public Sensor(int id, Monitor monitor){
        this.id = id;
        this.delay = 1000;
        this.monitor = monitor;
        this.readCounter = 0;
    }

    //Generates random numbers between [25,40]
    private int read(){
        Random readValue = new Random();
        int n = readValue.nextInt(16);
        n += 26;
        return n;
    }

    //Updates the number of times that the sensor has read something
    private void updateReadCounter(){
        this.readCounter++;
    }

    //Applies the main logic of the sensor, by reading temperatures at certain delay and range and guaranteeing the sync
    public void run(){
        try{
            while(true){
                this.monitor.sensorIn();
                int temperature = this.read();
                //System.out.println("Leu do " +this.id);
                if(temperature > 30){
                    this.updateReadCounter();
                    int i = this.monitor.getNextOverwrite();
                    this.monitor.data[i].input(this.id, temperature, this.readCounter);
                }
                this.monitor.sensorOut();
                sleep(this.delay);
            }
        } catch (InterruptedException e) { }
    }

}