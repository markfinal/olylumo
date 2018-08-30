using Bam.Core;
namespace olylumogui
{
    sealed class olylumoGUI :
        C.Cxx.GUIApplication
    {
        protected override void Init(Module parent)
        {
            base.Init(parent);

            var source = this.CreateCxxSourceContainer("$(packagedir)/source/*.cpp");

            if (this.BuildEnvironment.Platform.Includes(Bam.Core.EPlatform.OSX))
            {
                this.CompileAndLinkAgainst<Qt.CoreFramework>(source);
            }
            else
            {
                this.CompileAndLinkAgainst<Qt.Core>(source);
            }
        }
    }
}
