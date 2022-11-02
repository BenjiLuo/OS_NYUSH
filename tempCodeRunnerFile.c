pid_t pid, wpid;
                int status;

                pid = fork();
                if (pid == 0) {
                        // Child process
                        if (execvp(str1[0], str1[1]) == -1) {
                                perror("lsh");
                        }
                        exit(EXIT_FAILURE);
                }
                else if (pid < 0) {
                        // Error forking
                        perror("lsh");
                }
                else {
                        // Parent process
                        wait();
                        continue;
                }