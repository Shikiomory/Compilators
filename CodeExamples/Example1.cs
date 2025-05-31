using System;

/* 
   ������������� �����������
   ����� Calculator � ���������� ����������
*/
namespace CalculatorApp
{
    public class Calculator
    {
        // ����� ��� ���������� ����������
        public static int CalculateFactorial(int n)
        {
            if (n < 0)
                throw new ArgumentException("����� �� ����� ���� �������������");

            int result = 1;
            for (int i = 1; i <= n; i++)
            {
                result *= i;
            }
            return result;
        }

        public static double AddNumbers(double a, double b) => a + b;
    }

    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                Console.WriteLine("������� ����� ��� ���������� ����������:");
                string input = Console.ReadLine() ?? "5";
                int number = Convert.ToInt32(input);

                int factorial = Calculator.CalculateFactorial(number);
                Console.WriteLine($"��������� ����� {number} ����� {factorial}");

                double[] numbers = { 2.5, 3.7, 4.0 };
                double sum = 0.0;

                foreach (var num in numbers)
                {
                    sum = Calculator.AddNumbers(sum, num);
                }

                Console.WriteLine(@$"����� ��������� �������: 
{sum.ToString("F2")}");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"������: {ex.Message}");
            }
        }
    }
}