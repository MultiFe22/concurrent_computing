//This class models all the main methods of the actuator and creates all the required parameters

class Actuator extends Thread {
    private int id;
    private int delay;
    private Monitor monitor; //Share the same monitor with all the threads
    public Actuator(int id, Monitor monitor){
        this.delay = 2000;
        this.id = id;
        this.monitor = monitor;
    }

    //Define all the signs of the actuator
    private void redSign(){ System.out.println("RED ALERT"); }

    private void yellowSign(){ System.out.println("YELLOW ALERT"); }

    private void normalSign(){ System.out.println("NORMAL ALERT"); }

    //Applies the main logic of the sensor, by interpreting temperatures at certain delay and value, guaranteeing the right signs at the right time
    public void run(){
        try{
            while(true){
                this.monitor.actuatorIn();
                int count = 0;
                int sequence = 0; //Number of valid readings in the for loop
                int signal = 0; //Check if a signal was already displayed
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
