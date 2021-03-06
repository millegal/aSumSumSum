﻿using System.Globalization;
using System.Windows.Controls;

namespace com_b_velop.Rules
{
    public class NeuronRule : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            long neurons;
            if (long.TryParse(value.ToString(), out neurons))
            {
                if(neurons >= 20)
                    return ValidationResult.ValidResult;
                return new ValidationResult(false, "Zu wenig Neuronen");
            }
            return new ValidationResult(false, "Zu wenig Neuronen");
        }
    }
}
