using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;

namespace WPFStudy.TestBinding
{
    internal class Photo
    {
        public Photo(string path) { ImagePath = path; }
        public string ImagePath { get; set; }
    }

    internal class Photos : ObservableCollection<Photo>
    {
    }
}
