﻿using System;
using System.Windows;
using System.Windows.Input;
using com_b_velop.OpenFile;
using Microsoft.Practices.ServiceLocation;
using OpenFileView = com_b_velop.OpenFile.OpenFileView;

namespace com_b_velop.Commands
{
    public class OpenSourceCommand : ICommand
    {
        private readonly IServiceLocator _serviceLocater;

        public OpenSourceCommand(IServiceLocator serviceLocator)
        {
            _serviceLocater = serviceLocator;
        }
        public bool CanExecute(object parameter) => true;

        public void Execute(object parameter)
        {
            var width = Properties.Settings.Default.OpenFileWidth;
            var height = Properties.Settings.Default.OpenFileHeigth;
            var win = _serviceLocater.GetInstance<OpenFileView>();


            Properties.Settings.Default.OpenFileHeigth = win.ActualHeight;
            Properties.Settings.Default.OpenFileWidth = win.ActualWidth;
            Properties.Settings.Default.Save();
        }

        public event EventHandler CanExecuteChanged;
    }
}
