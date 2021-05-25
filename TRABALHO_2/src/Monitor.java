//The monitor class guarantees the thread-safe and the code requisites

class Monitor{
    private int sensorQueue, actuatorOn, sensorOn, nextOverwrite;
    public Data[] data = new Data[60];
    public Monitor(){
        this.sensorQueue = 0;
        this.actuatorOn = 0;
        this.actuatorOn = 0;
        this.nextOverwrite = 0;
        for(int i = 0; i < 60; i++){
            this.data[i] = new Data();
        }
    }

    //Calculates the next position to be Overwrite, using the FIFO logic
    public synchronized int getNextOverwrite(){
        this.nextOverwrite++;
        this.nextOverwrite = this.nextOverwrite%60;
        return this.nextOverwrite;
    }

    //This method applies the writer priority logic, by only blocking the sensor thread if a sensor is on or a single actuator
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

    //This method applies the writer priority logic, by blocking the actuator thread if a sensor is or or in the queue, prioritizing the sensor
    public synchronized void actuatorIn(){
        try{
            while(this.sensorQueue > 0 || this.sensorOn > 0){
                wait();
            }
            this.actuatorOn++;
        } catch (InterruptedException e) { }
    }

    //Ends the thread's work and notify all the blocked threads
    public synchronized void sensorOut(){
        this.sensorOn--;
        notifyAll();
    }

    //Ends the thread's work and notify all the blocked threads
    public synchronized void actuatorOut(){
        this.actuatorOn--;
        notifyAll();

    }


}