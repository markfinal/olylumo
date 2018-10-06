using Bam.Core;
using QtCommon.MocExtension;
using QtCommon.RccExtension;
namespace olylumogui
{
    class olylumoGUI :
        C.Cxx.GUIApplication
    {
        protected override void Init(Module parent)
        {
            base.Init(parent);

            var headers = this.CreateHeaderContainer("$(packagedir)/source/*.h");
            var source = this.CreateCxxSourceContainer("$(packagedir)/source/*.cpp");

            foreach (var header in headers.Children)
            {
                source.MocHeader(header);
            }

            var qrcFiles = this.CreateQrcContainer("$(packagedir)/resources/*.qrc");
            foreach (var qrc in qrcFiles.Children)
            {
                source.Rcc(qrc);
            }

            this.CompileAndLinkAgainst<olylumoray.RayTrace>(source);

            source.PrivatePatch(settings =>
            {
                if (settings is C.ICxxOnlyCompilerSettings cxxCompiler)
                {
                    cxxCompiler.EnableRunTimeTypeInfo = false;
                    cxxCompiler.LanguageStandard = C.Cxx.ELanguageStandard.Cxx11;
                    cxxCompiler.StandardLibrary = C.Cxx.EStandardLibrary.libcxx;
                }

                if (settings is VisualCCommon.ICommonCompilerSettings vcCompiler)
                {
                    vcCompiler.WarningLevel = VisualCCommon.EWarningLevel.Level4;
                }
                else if (settings is ClangCommon.ICommonCompilerSettings clangCompiler)
                {
                    clangCompiler.AllWarnings = true;
                    clangCompiler.ExtraWarnings = true;
                    clangCompiler.Pedantic = true;
                }
            });

            if (this.BuildEnvironment.Platform.Includes(Bam.Core.EPlatform.OSX))
            {
                this.CompileAndLinkAgainst<Qt.WidgetsFramework>(source);
                this.CompileAndLinkAgainst<Qt.XmlFramework>(source);
                this.LinkAgainst<Qt.GuiFramework>();
                this.LinkAgainst<Qt.CoreFramework>();
            }
            else
            {
                this.CompileAndLinkAgainst<Qt.Widgets>(source);
                this.CompileAndLinkAgainst<Qt.Xml>(source);
                this.LinkAgainst<Qt.Gui>();
                this.LinkAgainst<Qt.Core>();
            }

            this.PrivatePatch(settings =>
            {
                if (this.Linker is VisualCCommon.LinkerBase)
                {
                    var linker = settings as C.ICommonLinkerSettings;
                    linker.Libraries.Add("shell32.lib"); // for CommandLineToArgvW
                }
                else if (settings is ClangCommon.ICommonLinkerSettings clangLinker)
                {
                    clangLinker.RPath.AddUnique("@executable_path/../Frameworks/");
                }
            });

            this.ClosingPatch(settings =>
            {
                if (source.Settings is VisualCCommon.ICommonCompilerSettings vcCompiler)
                {
                    var linker = settings as C.ICommonLinkerSettings;
                    if (vcCompiler.RuntimeLibrary == VisualCCommon.ERuntimeLibrary.MultiThreadedDLL)
                    {
                        linker.Libraries.AddUnique("qtmain.lib");
                    }
                    else if (vcCompiler.RuntimeLibrary == VisualCCommon.ERuntimeLibrary.MultiThreadedDebugDLL)
                    {
                        linker.Libraries.AddUnique("qtmaind.lib");
                    }
                    else
                    {
                        throw new Bam.Core.Exception(
                            "Unsupported runtime library, {0}",
                            vcCompiler.RuntimeLibrary.ToString()
                        );
                    }
                }
            });
        }
    }

    sealed class olylumoGUIRuntime :
        Publisher.Collation
    {
        protected override void
        Init(
            Module parent)
        {
            base.Init(parent);

            this.SetDefaultMacrosAndMappings(EPublishingType.WindowedApplication);

            this.Include<olylumoGUI>(C.Cxx.GUIApplication.ExecutableKey);
        }
    }
}
