//This class is supposed to model the data stored in the shared memory, containing all the requisites

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