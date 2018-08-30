using Bam.Core;
namespace olylumogui
{
    sealed class OlyLumoGUI :
        C.Cxx.GUIApplication
    {
        protected override void Init(Module parent)
        {
            base.Init(parent);

            var source = this.CreateCxxSourceContainer("$(packagedir)/source/*.cpp");
            this.CompileAndLinkAgainst<Qt.Core>(source);
        }
    }
}
