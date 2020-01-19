/* BrainfucKompiler: a brainfuck compiler.
 * Copyright (C) 2020 Marçal Comajoan Cara
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "BrainfucKompiler: a brainfuck compiler.\n"
                 "Copyright (C) 2020 Marçal Comajoan Cara\n"
                 "\n"
                 "Usage:\n" << argv[0] << " file.bf\n";
    return 1;
  }

  std::string input_filename = argv[1];
  size_t last_dot_position = input_filename.find_last_of('.');
  if (last_dot_position == std::string::npos ||
      input_filename.substr(last_dot_position) != ".bf") {
    std::cout << "ERROR: File type not recognized. "
                 "Remember to use the .bf extension.";
    return 1;
  }

  std::fstream input_file(input_filename, std::fstream::in);

  std::string c_filename = input_filename.substr(0, last_dot_position) + ".c";
  std::fstream c_file(c_filename, std::fstream::out);

  c_file << "#include <stdio.h>\n"
            "\n"
            "int main() {\n"
            "\tchar array[30000] = {0};\n"
            "\tchar *ptr = array;\n";

  size_t indentation_level = 1;

  char c;
  while (input_file.get(c)) {
    switch (c) {
      case '>':
        c_file << std::string(indentation_level, '\t') << "++ptr;\n";
        break;
      case '<':
        c_file << std::string(indentation_level, '\t') << "--ptr;\n";
        break;
      case '+':
        c_file << std::string(indentation_level, '\t') << "++*ptr;\n";
        break;
      case '-':
        c_file << std::string(indentation_level, '\t') << "--*ptr;\n";
        break;
      case '.':
        c_file << std::string(indentation_level, '\t') << "putchar(*ptr);\n";
        break;
      case ',':
        c_file << std::string(indentation_level, '\t') << "*ptr = getchar();\n";
        break;
      case '[':
        c_file << std::string(indentation_level, '\t') << "while (*ptr) {\n";
        ++indentation_level;
        break;
      case ']':
        --indentation_level;
        c_file << std::string(indentation_level, '\t') << "}\n";
        break;
    }
  }

  c_file << "}\n";

  input_file.close();
  c_file.close();

  std::system(("cc " + c_filename + " -O3 -o " +
      input_filename.substr(0, last_dot_position)).c_str());

  return 0;
}

