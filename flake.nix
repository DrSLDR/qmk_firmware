{
  description = "QMK shell flake";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/98b00b6947a9214381112bdb6f89c25498db4959";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShell = (import ./shell.nix) {
          inherit pkgs;
          arm = false;
          teensy = false;
        };
      }
    );
}
