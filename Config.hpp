/* Copyright (c) 2023 Caio Souza, Guilherme Martinelli, Luigi Ferrari. */
/* All rights reserved. 42 */

namespace ft {
    class Config {
        public:
            Config(void);
            Config(std::string const filename);
            ~Config(void);
        private:
            std::string _filename;
    }; // class Config
} // namespace ft
