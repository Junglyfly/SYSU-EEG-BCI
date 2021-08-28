
import sys,traceback
try:
    import unittest
    import pystyx
except:
    print traceback.print_exc()
    sys.stdin.readline()
    raise

class TestLocal(unittest.TestCase):
    def setUp(self):
        self.conn=pystyx.LocalConnection()
        self.client=pystyx.Client(self.conn)

    def checkRoot(self,d):
        self.assertEqual(len(d),6,"files in root")
        files=['msg','inc','str','no_open']
        for fn in files:
            self.failUnless(fn in d.names(),"%s not found in %s"%(fn,d.path()))

    def testCWD(self):
        c=self.client
        root=pystyx.Directory(c,'/')
        self.checkRoot(root)
        cwd=pystyx.Directory(c)
        self.assertEqual('/',cwd.path(),"cwd path should be /")
        self.checkRoot(cwd)

try:
    suite = unittest.makeSuite(TestLocal)
    unittest.TextTestRunner(verbosity=2).run(suite)

except:
    print "exception"
    print traceback.print_exc()

sys.stdin.readline()
