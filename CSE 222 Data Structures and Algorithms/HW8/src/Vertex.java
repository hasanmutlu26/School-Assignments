public class Vertex{
    private String label;
    private double weight;

    public Vertex(){
        label = new String();
        weight = Double.POSITIVE_INFINITY;
    }

    public Vertex(String label, double weight){
        this.label = new String(label);
        this.weight = weight;
    }

    public String getLabel(){
        return label;
    }

    public void setLabel(String label){
        this.label = new String(label);
    }

    public double getWeight(){
        return weight;
    }

    public void setWeight(double weight){
        this.weight = weight;
    }
}