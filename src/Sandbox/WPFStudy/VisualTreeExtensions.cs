using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Media;
using System.Threading;
using System.Windows.Threading;

namespace WPFStudy
{
    public static class VisualTreeHelperExtension
    {
        /// <summary>
        /// Depth-first, preorder traversal
        /// </summary>
        /// <param name="node"></param>
        /// <returns></returns>
        public static IEnumerable<Visual> Descendants(this Visual node)
        {
            for (int i = 0; i < VisualTreeHelper.GetChildrenCount(node); i++)
            {
                var child = (Visual)VisualTreeHelper.GetChild(node, i);
                yield return child;

                foreach (var childOfChild in Descendants(child))
                {
                    yield return childOfChild;
                }
            }
        }

        public static IEnumerable<Visual> Ancestors(this Visual node)
        {
            var parent = (Visual)VisualTreeHelper.GetParent(node);
            if (parent == null)
            {
                yield break;
            }

            yield return parent;

            foreach (var parentOfParent in Ancestors(parent))
            {
                yield return parentOfParent;
            }
        }

        public static IEnumerable<Visual> Siblings(this Visual node)
        {
            var parent = (Visual)VisualTreeHelper.GetParent(node);
            if (parent == null)
            {
                yield break;
            }

            for (int i = 0; i < VisualTreeHelper.GetChildrenCount(parent); i++)
            {
                var child = (Visual)VisualTreeHelper.GetChild(parent, i);
                yield return child;
            }
        }
    }
}

