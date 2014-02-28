/* 
    This file is part of yamcr, an experimental Monte Carlo renderer

    Copyright (c) 2013-2014 by Tzu-Mao Li

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

using namespace yamcr;

int main(int argc, char *argv[]) {
    try {
        rtcInit(NULL);
        for(int i = 1; i < argc; i++) {
            SceneParser parser;
            std::shared_ptr<Renderer> renderer = parser.Parse(std::string(argv[i]));
            renderer->Render();
        }
        rtcExit();
    } catch(std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }
    return 0;
}
