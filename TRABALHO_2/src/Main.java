//The main works executing the basic logic of the code, it starts all the threads and keep them executing

class Main {
    public static void main(String[] args) {
        int sensorNum = Integer.parseInt(args[0]); //Transforms the command line argument string into an integer
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