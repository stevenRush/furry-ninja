using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;

namespace WinForms
{
    enum ExpressionType
    {
        Expression,
        Variable,
        Number,
        OneArgumentFunction,
        TwoArgumentFunction
    }

    class Node
    {
        public string Operation;
        public double Value;
        public ExpressionType Type;
        public Node Left;
        public Node Right;
    }

    class SolverUtilities
    {
        /// <summary>
        /// Removes external bracets in such cases: (a+b) or ((a+b)*c)
        /// </summary>
        /// <param name="ex"></param>
        public static void RemoveExternalBracets(ref string ex)
        {
            int balance = 0;
            int i;
            if (ex[0] != '(')
                return;
            balance = 1;
            for (i = 1; i < ex.Count() && balance > 0; i++)
            {
                if (ex[i] == '(')
                    balance++;
                if (ex[i] == ')')
                    balance--;
            }
            if (i == ex.Count())
                ex = ex.Substring(1, ex.Length - 2);
        }

        /// <summary>
        /// Checks if last operation in the expression is function. For example: sin(x) or cos(15*x+12)
        /// </summary>
        /// <param name="ex"></param>
        /// <returns></returns>
        public static bool CheckFunction(string ex)
        {
            string pattern = @"^[\w]+[(]{1}.*[)]{1}$";
            return Regex.IsMatch(ex, pattern);
        }

        /// <summary>
        /// Defines number of arguments function takes
        /// </summary>
        /// <param name="ex"></param>
        /// <returns></returns>
        public static int NumberOfFunctionArguments(string ex)
        {
            string pattern = @"^.*[,]{1}.*$";
            return Regex.IsMatch(ex, pattern) ? 2 : 1;
        }

        /// <summary>
        /// Checks if string contains only a number
        /// </summary>
        /// <param name="ex"></param>
        /// <returns></returns>
        public static bool CheckNumber(string ex)
        {
            string pattern = @"^[0-9]+$";
            return Regex.IsMatch(ex, pattern);
        }

        /// <summary>
        /// Checks if string contains only a variable
        /// </summary>
        /// <param name="ex"></param>
        /// <returns></returns>
        public static bool CheckVariable(string ex)
        {
            string pattern = @"^[a-zA-Z]+$";
            return Regex.IsMatch(ex, pattern);
        }    

        /// <summary>
        /// Finds an "top level" entry of any of these operator signs
        /// </summary>
        /// <param name="ex"></param>
        /// <param name="sign"></param>
        /// <returns></returns>
        public static int FindOperationSign(string ex, string[] signs)
        {
            int balance = 0;
            for(int i=0; i < ex.Count(); i++)
            {
                if (ex[i] == '(')
                    balance++;
                if (ex[i] == ')')
                    balance--;
                if (signs.Contains(ex[i].ToString()) && balance == 0)
                    return i;
            }
            return -1;
        }

        public static bool CheckBracets(string ex)
        {
            int balance = 0;
            for (int i = 0; i < ex.Count() && balance > -1; i++)
            {
                if (ex[i] == '(')
                    balance++;
                if (ex[i] == ')')
                    balance--;
            }
            return balance == 0;
        }

        public static double ExecuteExpression(string operation, double arg1, double arg2)
        {
            double result = 0.0;
            switch (operation)
            {
                case "+":
                    result = arg1 + arg2;
                    break;
                case "-":
                    result = arg1 - arg2;
                    break;
                case "*":
                    result = arg1 * arg2;
                    break;
                case "/":
                    if (Math.Abs(arg2) < double.Epsilon)
                        result = double.NaN;
                    else
                        result = arg1 / arg2;
                    break;
                case "^":
                    result = Math.Pow(arg1, arg2);
                    break;
            }
            return result;
        }

        public static double ExecuteFunction(string function, double arg1)
        {
            double result = 0.0;
            switch (function)
            {
                case "abs":
                    result = Math.Abs(arg1);
                    break;
                case "acos":
                case "arccos":
                    result = Math.Acos(arg1);
                    break;
                case "asin":
                case "arcsin":
                    result = Math.Asin(arg1);
                    break;
                case "atan":
                case "arctg":
                    result = Math.Atan(arg1);
                    break;
                case "cos":
                    result = Math.Cos(arg1);
                    break;
                case "sin":
                    result = Math.Sin(arg1);
                    break;
                case "tan":
                case "tg":
                    result = Math.Tan(arg1);
                    break;
                case "cosh":
                case "ch":
                    result = Math.Cosh(arg1);
                    break;
                case "sinh":
                case "sh":
                    result = Math.Sinh(arg1);
                    break;
                case "tanh":
                case "th":
                    result = Math.Tanh(arg1);
                    break;
                case "exp":
                    result = Math.Exp(arg1);
                    break;
                case "ln":
                case "log":
                    result = Math.Log(arg1);
                    break;
                case "log10":
                    result = Math.Log10(arg1);
                    break;
                case "sign":
                    result = Math.Sign(arg1);
                    break;
                case "sqrt":
                    result = Math.Sqrt(arg1);
                    break;
            }
            return result;
        }

        public static double ExecuteFunction(string function, double arg1, double arg2)
        {
            double result = 0.0;
            switch (function)
            {
                case "pow":
                    result = Math.Pow(arg1, arg2);
                    break;
            }

            return result;
        }
    }

    /// <summary>
    /// Expression solver class. 
    /// Features:
    /// 1) You can solve any correct expression you want
    /// 2) Variables. You can use variables in expressions. Just write "x+y" and then you can change x's and y's values.
    ///     If you change variable's value, expression wont be parsed. It gives big advantage in calculation speed
    /// </summary>
    class ExpressionSolver
    {
        private bool m_IsCompiled;

        private string m_Expression;
        private Dictionary<string, double> m_Variables;
        private Node ExpressionTree;

        /// <summary>
        /// String representation of expression
        /// </summary>
        public string Expression
        {
            set
            {
                m_Expression = value;
                m_IsCompiled = false;
            }
            get
            {
                return m_Expression;
            }
        }

        /// <summary>
        /// Expression's value after executing
        /// </summary>
        public double Result
        {
            get;
            set;
        }

        public ExpressionSolver()
        {
            m_Variables = new Dictionary<string, double>();
            m_Variables.Add("pi", Math.PI);
            m_Variables.Add("e", Math.E);
            m_IsCompiled = false;

        }    

        /// <summary>
        /// Makes expression tree from string
        /// </summary>
        /// <param name="ex"></param>
        /// <returns></returns>
        private Node MakeTree(string ex)
        {
            SolverUtilities.RemoveExternalBracets(ref ex);
            Node res = new Node();

            if (SolverUtilities.CheckNumber(ex))
            {
                res.Operation = ex;
                res.Type = ExpressionType.Number;
                res.Left = null;
                res.Right = null;
            }

            if (SolverUtilities.CheckVariable(ex))
            {
                res.Operation = ex;
                res.Type = ExpressionType.Variable;
                res.Left = null;
                res.Right = null;
            }

            if (SolverUtilities.CheckFunction(ex))
            {
                int openBracet, closeBracet;

                openBracet = ex.IndexOf("(");
                closeBracet = ex.LastIndexOf(")");
                res.Operation = ex.Substring(0, openBracet);
                string FunctionParam = ex.Substring(openBracet+1, closeBracet-openBracet-1);
                // If function takes one argument then make one child node. 
                // If it takes two argumetns then make two children nodes; 
                if (SolverUtilities.NumberOfFunctionArguments(FunctionParam) == 1)
                {
                    res.Type = ExpressionType.OneArgumentFunction;
                    res.Left = MakeTree(FunctionParam);
                    res.Right = null;
                }
                else
                {
                    int comma = FunctionParam.IndexOf(",");
                    res.Type = ExpressionType.TwoArgumentFunction;
                    res.Left = MakeTree(FunctionParam.Substring(0, comma));
                    res.Right = MakeTree(FunctionParam.Substring(comma+1, FunctionParam.Length-comma-1));
                }
            }

            // We should pay attention to operations' priorities
            int signIndex = SolverUtilities.FindOperationSign(ex, new string[] {"-", "+"});
            if (signIndex == -1)
                signIndex = SolverUtilities.FindOperationSign(ex, new string[] {"/", "*"});
            if (signIndex == -1)
                signIndex = SolverUtilities.FindOperationSign(ex, new string[] { "^" });
            if (signIndex != -1)
            {
                res.Operation = ex[signIndex].ToString();
                res.Type = ExpressionType.Expression;
                res.Left = MakeTree(ex.Substring(0, signIndex));
                res.Right = MakeTree(ex.Substring(signIndex+1, ex.Length-signIndex-1));
            }

            return res;
        }

        /// <summary>
        /// Parsing string expression into expression tree
        /// </summary>
        public void Prepare()
        {
            // Check bracet balance
            if (!SolverUtilities.CheckBracets(m_Expression))
                return;

            // Removing all spaces and make all characters low
            m_Expression = m_Expression.ToLower().Replace(" ", string.Empty);

            // Building an expression tree
            ExpressionTree = MakeTree(m_Expression);

            m_IsCompiled = true;
        }

        public void SetVariable(string variable, double value)
        {
            variable = variable.ToLower();
            if (m_Variables.ContainsKey(variable))
                m_Variables[variable] = value;
            else
                m_Variables.Add(variable, value);
        }

        /// <summary>
        /// Figures out a value of given expression tree node
        /// </summary>
        /// <param name="tree"></param>
        public void ExecuteTree(Node tree)
        {
            if (tree == null)
                return;

            ExecuteTree(tree.Left);
            ExecuteTree(tree.Right);

            switch (tree.Type)
            {
                case ExpressionType.Number:
                    tree.Value = double.Parse(tree.Operation);
                    break;
                case ExpressionType.Variable:
                    if (!m_Variables.ContainsKey(tree.Operation))
                        return;
                    tree.Value = m_Variables[tree.Operation];
                    break;
                case ExpressionType.Expression:
                    tree.Value = SolverUtilities.ExecuteExpression(tree.Operation, tree.Left.Value, tree.Right.Value);
                    break;
                case ExpressionType.OneArgumentFunction:
                    tree.Value = SolverUtilities.ExecuteFunction(tree.Operation, tree.Left.Value);
                    break;
                case ExpressionType.TwoArgumentFunction:
                    tree.Value = SolverUtilities.ExecuteFunction(tree.Operation, tree.Left.Value, tree.Right.Value);
                    break;
            }
        }

        /// <summary>
        /// Figures out a value of whole expression
        /// </summary>
        public void Execute()
        {
            if (!m_IsCompiled)
                Prepare();
            ExecuteTree(ExpressionTree);
            Result = ExpressionTree.Value;
        }
    }
}
