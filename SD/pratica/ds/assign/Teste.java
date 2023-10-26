package ds.assign;

import java.util.ArrayList;

public class Teste
{
    public static void main(String[] args) throws Exception
    {
        ArrayList<Double> arr = new ArrayList<Double>() ;
        arr.add(2.5);
        arr.add(3.3);
        arr.add(4.2);
        String str = "";
        int tamanho = arr.size();
        for (int i  = 0; i < tamanho-1; i++)
        {
            str+= arr.get(i) +"," ;
        }
        str+= arr.get(tamanho-1);
        System.out.println("Array as string");
        System.out.println(str);

        ArrayList<Double> newArr = new ArrayList<Double>() ;

        String[] numbers = str.split(",");
        for (String x : numbers)
        {
            System.out.println(x);
            newArr.add(Double.parseDouble(x));
        }
        String str2 = "";
        tamanho = newArr.size();
        for (int i  = 0; i < tamanho-1; i++)
        {
            str2+= newArr.get(i) +"," ;
        }
        str2+= newArr.get(tamanho-1);

        System.out.println("Array as string");
        System.out.println(str2);
    }
}