using System;

/* 
   многострочный комментарий
   Класс Calculator с различными операциями
*/
namespace CalculatorApp
{
    public class Calculator
    {
        // Метод для вычисления факториала
        public static int CalculateFactorial(int n)
        {
            if (n < 0)
                throw new ArgumentException("Число не может быть отрицательным");

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
                Console.WriteLine("Введите число для вычисления факториала:");
                string input = Console.ReadLine() ?? "5";
                int number = Convert.ToInt32(input);

                int factorial = Calculator.CalculateFactorial(number);
                Console.WriteLine($"Факториал числа {number} равен {factorial}");

                double[] numbers = { 2.5, 3.7, 4.0 };
                double sum = 0.0;

                foreach (var num in numbers)
                {
                    sum = Calculator.AddNumbers(sum, num);
                }

                Console.WriteLine(@$"Сумма элементов массива: 
{sum.ToString("F2")}");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Ошибка: {ex.Message}");
            }
        }
    }
}
