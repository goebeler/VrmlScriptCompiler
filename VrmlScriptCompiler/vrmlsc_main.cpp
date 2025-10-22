#include "ast_json.hpp"
#include "driver.hh"
#include <fstream>
#include <iostream>
#include <string>


using vrmlast::Script;

static int parse_from_string(const std::string &code, vrmlast::ASTNode *&outRoot) {
  driver drv;
  
  int rc = drv.parse_string(code);
  outRoot = (rc == 0) ? drv.m_root : nullptr;
  return rc;
}

int main(int argc, char **argv) {
  bool ast_json = false;
  std::string path;

  for (int i = 1; i < argc; ++i) {
    std::string a = argv[i];
    if (a == "--ast-json")
      ast_json = true;
    else
      path = a;
  }

  std::string code;
  if (!path.empty() && path != "-") {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) {
      std::cerr << "cannot open: " << path << "\n";
      return 2;
    }
    code.assign(std::istreambuf_iterator<char>(ifs),
                std::istreambuf_iterator<char>());
  } else {
    // stdin lesen
    code.assign(std::istreambuf_iterator<char>(std::cin),
                std::istreambuf_iterator<char>());
  }

  vrmlast::ASTNode *root = nullptr;
  if (int rc = parse_from_string(code, root)) {
    std::cerr << "parse failed (" << rc << ")\n";
    return rc;
  }

  if (ast_json) {
    vrmlast::JsonAstVisitor json_visitor{};
    root->accept(json_visitor);
    auto j = json_visitor.take();
    std::cout << j.dump(2) << "\n";
  } else {
    std::cout << "OK\n";
  }
  return 0;
}
