from conans import ConanFile

class MyConsumer(ConanFile):

    def source(self):
        self.run("git clone https://github.com/username/foo.git")