using System;
using System.Diagnostics;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Xnor.Core;

namespace CoreCSharpTests
{
    [TestClass]
    public class Color
    {
        [TestClass]
        public class ConversionRgbaHsva
        {
            [TestMethod]
            public void Red()
            {
                ColorRgba rgba = ColorRgba.Red;
                ColorHsva hsva = ColorHsva.Red;
            
                Assert.AreEqual(hsva, (ColorHsva) rgba);
                Assert.AreEqual(rgba, (ColorRgba) hsva);
            }
            
            [TestMethod]
            public void Green()
            {
                ColorRgba rgba = ColorRgba.Green;
                ColorHsva hsva = ColorHsva.Green;
            
                Assert.AreEqual(hsva, (ColorHsva) rgba);
                Assert.AreEqual(rgba, (ColorRgba) hsva);
            }
            
            [TestMethod]
            public void Blue()
            {
                ColorRgba rgba = ColorRgba.Blue;
                ColorHsva hsva = ColorHsva.Blue;
            
                Assert.AreEqual(hsva, (ColorHsva) rgba);
                Assert.AreEqual(rgba, (ColorRgba) hsva);
            }
            
            [TestMethod]
            public void Yellow()
            {
                ColorRgba rgba = ColorRgba.Yellow;
                ColorHsva hsva = ColorHsva.Yellow;
            
                Assert.AreEqual(hsva, (ColorHsva) rgba);
                Assert.AreEqual(rgba, (ColorRgba) hsva);
            }
            
            [TestMethod]
            public void Magenta()
            {
                ColorRgba rgba = ColorRgba.Magenta;
                ColorHsva hsva = ColorHsva.Magenta;
            
                Assert.AreEqual(hsva, (ColorHsva) rgba);
                Assert.AreEqual(rgba, (ColorRgba) hsva);
            }
            
            [TestMethod]
            public void LightBlue()
            {
                ColorRgba rgba = ColorRgba.LightBlue;
                ColorHsva hsva = ColorHsva.LightBlue;
            
                Assert.AreEqual(hsva, (ColorHsva) rgba);
                Assert.AreEqual(rgba, (ColorRgba) hsva);
            }
            
            [TestMethod]
            public void Black()
            {
                ColorRgba rgba = ColorRgba.Black;
                ColorHsva hsva = ColorHsva.Black;
            
                Assert.AreEqual(hsva, (ColorHsva) rgba);
                Assert.AreEqual(rgba, (ColorRgba) hsva);
            }
            
            [TestMethod]
            public void White()
            {
                ColorRgba rgba = ColorRgba.White;
                ColorHsva hsva = ColorHsva.White;
            
                Assert.AreEqual(hsva, (ColorHsva) rgba);
                Assert.AreEqual(rgba, (ColorRgba) hsva);
            }
        }
        
        [TestClass]
        public class ConversionRgbaf
        {
            [TestMethod]
            public void Red()
            {
                ColorRgba rgba = ColorRgba.Red;
                Colorf hsva = Colorf.Red;
            
                Assert.AreEqual(hsva, (Colorf) rgba);
                Assert.AreEqual(rgba, (ColorRgba) hsva);
            }
            
            [TestMethod]
            public void Green()
            {
                ColorRgba rgba = ColorRgba.Green;
                Colorf hsva = Colorf.Green;
            
                Assert.AreEqual(hsva, (Colorf) rgba);
                Assert.AreEqual(rgba, (ColorRgba) hsva);
            }
            
            [TestMethod]
            public void Blue()
            {
                ColorRgba rgba = ColorRgba.Blue;
                Colorf hsva = Colorf.Blue;
            
                Assert.AreEqual(hsva, (Colorf) rgba);
                Assert.AreEqual(rgba, (ColorRgba) hsva);
            }
            
            [TestMethod]
            public void Yellow()
            {
                ColorRgba rgba = ColorRgba.Yellow;
                Colorf hsva = Colorf.Yellow;
            
                Assert.AreEqual(hsva, (Colorf) rgba);
                Assert.AreEqual(rgba, (ColorRgba) hsva);
            }
            
            [TestMethod]
            public void Magenta()
            {
                ColorRgba rgba = ColorRgba.Magenta;
                Colorf hsva = Colorf.Magenta;
            
                Assert.AreEqual(hsva, (Colorf) rgba);
                Assert.AreEqual(rgba, (ColorRgba) hsva);
            }
            
            [TestMethod]
            public void LightBlue()
            {
                ColorRgba rgba = ColorRgba.LightBlue;
                Colorf hsva = Colorf.LightBlue;
            
                Assert.AreEqual(hsva, (Colorf) rgba);
                Assert.AreEqual(rgba, (ColorRgba) hsva);
            }
            
            [TestMethod]
            public void Black()
            {
                ColorRgba rgba = ColorRgba.Black;
                Colorf hsva = Colorf.Black;
            
                Assert.AreEqual(hsva, (Colorf) rgba);
                Assert.AreEqual(rgba, (ColorRgba) hsva);
            }
            
            [TestMethod]
            public void White()
            {
                ColorRgba rgba = ColorRgba.White;
                Colorf hsva = Colorf.White;
            
                Assert.AreEqual(hsva, (Colorf) rgba);
                Assert.AreEqual(rgba, (ColorRgba) hsva);
            }
        }
        
        [TestMethod]
        public void EqualityRgb()
        {
            Assert.AreEqual(ColorRgb.LightBlue, ColorRgb.LightBlue);
        }
        
        [TestMethod]
        public void EqualityRgba()
        {
            Assert.AreEqual(ColorRgba.LightBlue, ColorRgba.LightBlue);
        }
        
        [TestMethod]
        public void Equalityf()
        {
            Assert.AreEqual(Colorf.LightBlue, Colorf.LightBlue);
        }
        
        [TestMethod]
        public void EqualityHsva()
        {
            Assert.AreEqual(ColorHsva.LightBlue, ColorHsva.LightBlue);
        }
    }
}
