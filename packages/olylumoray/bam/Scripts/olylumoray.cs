namespace olylumoray
{
    class RayTrace :
        C.Cxx.DynamicLibrary
    {
        protected override void
        Init()
        {
            base.Init();

            this.CreateHeaderCollection("$(packagedir)/include/olylumoray/*.h");

            var source = this.CreateCxxSourceCollection("$(packagedir)/source/*.cpp");
            source.PrivatePatch(settings =>
            {
                if (settings is C.ICommonPreprocessorSettings preprocessor)
                {
                    preprocessor.PreprocessorDefines.Add("USE_TBB");
                }
                if (settings is C.ICxxOnlyCompilerSettings cxxCompiler)
                {
                    cxxCompiler.ExceptionHandler = C.Cxx.EExceptionHandler.Asynchronous;
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
                else if (settings is GccCommon.ICommonCompilerSettings gccCompiler)
                {
                    gccCompiler.AllWarnings = true;
                    gccCompiler.ExtraWarnings = true;
                    gccCompiler.Pedantic = true;
                    gccCompiler.PositionIndependentCode = true;
                }
            });

            this.PublicPatch((settings, appliedTo) =>
            {
                if (settings is C.ICommonPreprocessorSettings preprocessor)
                {
                    preprocessor.IncludePaths.AddUnique(this.CreateTokenizedString("$(packagedir)/include"));
                }
            });

            this.PrivatePatch(settings =>
            {
                if (settings is C.ICxxOnlyLinkerSettings cxxLinker)
                {
                    cxxLinker.StandardLibrary = C.Cxx.EStandardLibrary.libcxx;
                }
            });

            this.CompileAndLinkAgainst<tbb.ThreadBuildingBlocks>(source);
        }
    }
}
