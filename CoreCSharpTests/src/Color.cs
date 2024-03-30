namespace CoreCSharpTests
{
    [TestClass]
    public class Color
    {
        [TestMethod]
        public void ConversionRgbaHsva()
        {
            ColorRgba rgba = ColorRgba.Magenta;
            ColorHsva hsva = ColorHsva.Magenta;
            
            Assert.AreEqual(hsva, (ColorHsva) rgba);
            Assert.AreEqual(rgba, (ColorRgba) hsva);
        }
        
        [TestMethod]
        public void ConversionRgbaf()
        {
            ColorRgba rgba = ColorRgba.Magenta;
            Colorf f = Colorf.Magenta;
            
            Assert.AreEqual(f, (Colorf) rgba);
            Assert.AreEqual(rgba, (ColorRgba) f);
        }
        
        [TestMethod]
        public void EqualityRgb()
        {
            Assert.AreEqual(ColorRgb.Cyan, ColorRgb.Cyan);
        }
        
        [TestMethod]
        public void EqualityRgba()
        {
            Assert.AreEqual(ColorRgba.Cyan, ColorRgba.Cyan);
        }
        
        [TestMethod]
        public void Equalityf()
        {
            Assert.AreEqual(Colorf.Cyan, Colorf.Cyan);
        }
        
        [TestMethod]
        public void EqualityHsva()
        {
            Assert.AreEqual(ColorHsva.Cyan, ColorHsva.Cyan);
        }
    }
}
