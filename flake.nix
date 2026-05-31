{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    nvf.url = "github:notashelf/nvf";
    import-tree.url = "github:vic/import-tree";
  };


  outputs = inputs: let
    pkgs = inputs.nixpkgs.legacyPackages.x86_64-linux;

    loadedPkgs = file: import file { inherit pkgs; };
    loadedPkgFiles = builtins.readDir ./flakeAssets/pkgs;
    shellPkgs = builtins.concatLists (
      map (f: loadedPkgs ./flakeAssets/pkgs/${f})
          (builtins.filter (f: loadedPkgFiles.${f} == "regular")
                           (builtins.attrNames loadedPkgFiles))    
    );
    neovim = inputs.nvf.lib.neovimConfiguration {
      inherit pkgs;
      modules = [
        { _module.args = { nvf = inputs.nvf; }; }
        (inputs.import-tree ./flakeAssets/nvim)
      ];
    };

    fhsEnv = pkgs.buildFHSEnv {
      name = "fhsShell";
      targetPkgs = pkgs: [ 
        pkgs.clang
        pkgs.clang-tools
        pkgs.cmake
        pkgs.libGL
        pkgs.wayland-scanner
        neovim.neovim
      ];

      profile = ''     
        PS1='\[\e[38;5;141;1m\][\[\e[38;5;111m\]dev-shell\[\e[38;5;112m\]@\[\e[38;5;111m\]nvim\[\e[38;5;112m\]:\[\e[38;5;219m\]\w\[\e[38;5;141m\]]\[\e[38;5;177m\]\\$\[\e[0m\] '
        echo 'This dev shell comes with custom neovim install. To use it run "nvim"'
      '';

      runScript = "bash";
    };

  in {



    devShells.x86_64-linux.default = fhsEnv.env;


    };
}
