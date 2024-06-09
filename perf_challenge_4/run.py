import sys
import subprocess
import os
import shutil
import argparse

# python run.py
# 
# Instructions:
# The script goes through all the folders in current dir and tries 
# to build and run each submission.
# Use this script to track the progress of your experiments.
# I recommend to create a new folder for each new experiment.
# When you run the script for all the submissions in the folder,
# It will generate the score table for each of your experiments.
# Example:
# time(s)  submission    timings for 10 consecutive runs (s)                              speedup
# ([3.56,   'AVX2',       [3.56, 3.56, 3.56, 3.56, 3.56, 3.56, 3.56, 3.56, 3.56, 3.56]], ' + 1.21x')
# ([4.29,   'baseline',   [4.29, 4.29, 4.29, 4.29, 4.3, 4.3, 4.3, 4.3, 4.3, 4.33]],      ' + 1.0x' )
# ([4.35,   'align loop', [4.35, 4.35, 4.35, 4.35, 4.35, 4.35, 4.35, 4.35, 4.35, 4.35]], ' + 0.99x')

parser = argparse.ArgumentParser(description='perf test')
parser.add_argument("-num", type=int, help="num to run", default=10)
args = parser.parse_args()

num_run = args.num

FNULL = open(os.devnull, 'w')

saveCwd = os.getcwd()

submissions = list(tuple())

print ("Submissions:")

for submission in os.listdir(os.getcwd()):
  if not submission == ".git" and not submission == ".vs":
    if not os.path.isfile(os.path.join(os.getcwd(), submission)):
      submissions.append((submission, os.path.join(os.getcwd(), submission)))
      print ("  " + submission)

print ("Building ...")

for submissionName, submissionDir in submissions:
  # if not submissionName == "canny_opt":
  #   print("skip others for now")
  #   continue

  print ("Building " + submissionName + " ...")

  submissionBuildDir = os.path.join(submissionDir, "build")
  if os.path.exists(submissionBuildDir):
    shutil.rmtree(submissionBuildDir)

  if not os.path.exists(submissionBuildDir):
    os.mkdir(submissionBuildDir)
  os.chdir(submissionBuildDir)    

  try:
    subprocess.check_call("cmake ../ -B . -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang", shell=True, stdout=FNULL, stderr=FNULL)
    print("  cmake - OK")
  except:
    print("  cmake - Failed")

  try:
    if sys.platform != 'win32':           
      subprocess.check_call("make", shell=True, stdout=FNULL, stderr=FNULL)
    else:
      subprocess.check_call("cmake --build . --target canny --config RelWithDebInfo", shell=True, stdout=FNULL, stderr=FNULL)
      subprocess.check_call("cmake --build . --target validate --config RelWithDebInfo", shell=True, stdout=FNULL, stderr=FNULL)
    print("  make - OK")
  except:
    print("  make - Failed")
  
  # if sys.platform == 'win32':
  #   try:
  #     subprocess.check_call("copy .\\Release\\*.exe .", shell=True, stdout=FNULL, stderr=FNULL)
  #     print("  copy .exe - OK")
  #   except:
  #     print("  copy .exe - Failed")

  inputFileName = str(os.path.join(saveCwd, "221575.pgm"))
  destDir = str(submissionBuildDir)
  try:
    if sys.platform != 'win32':                
      subprocess.check_call("cp " + inputFileName + " " + destDir, shell=True, stdout=FNULL, stderr=FNULL)
    else:
      subprocess.check_call("copy " + inputFileName + " " + destDir, shell=True, stdout=FNULL, stderr=FNULL)
    print("  copy image - OK")
  except:
    print("  copy image - Failed")

os.chdir(saveCwd)

print ("Running ...")

scoretable = []

baseline = float(0)

for submissionName, submissionDir in submissions:
  # if not submissionName == "canny_opt":
  #   print("skip others for now")
  #   continue

  print ("Running " + submissionName + " ...")

  submissionBuildDir = os.path.join(submissionDir, "build")
  os.chdir(submissionBuildDir)
  scores = []

  inputFileName = str(os.path.join(submissionBuildDir, "221575.pgm"))
  if sys.platform != 'win32':      
    runCmd = "./canny " + inputFileName + " 0.5 0.7 0.9 2>&1"
  else:
    runCmd = ".\\canny.exe " + inputFileName + " 0.5 0.7 0.9"
  valid = True
  try:
    subprocess.check_call(runCmd, shell=True, stdout=FNULL, stderr=FNULL)   
  except:
    valid = False

  if valid:
    print("  run - OK")
  if not valid:
    print("  run - Failed")
    print("  input file:" + inputFileName)
  
  outputFileName = str(os.path.join(submissionBuildDir, "221575.pgm_s_0.50_l_0.70_h_0.90.pgm"))
  goldenFileName = str(os.path.join(saveCwd, "221575-out-golden.pgm"))

  valid = True
  try:
    if sys.platform != 'win32':            
      subprocess.check_call("./validate " + goldenFileName + " " + outputFileName, shell=True, stdout=FNULL, stderr=FNULL)
    else:
      subprocess.check_call(".\\validate.exe " + goldenFileName + " " + outputFileName, shell=True, stdout=FNULL, stderr=FNULL)          
  except:
    valid = False
  
  if valid:
    print("  validation - OK")
  if not valid:
    print("  validation - Failed")
    print("  validation - validate.exe " + goldenFileName + " " + outputFileName)

  print ("Running {} measurements loop ...".format(num_run))
    
  for x in range(0, num_run):
    os.remove(outputFileName);
    if sys.platform != 'win32':
      output = subprocess.check_output("time -p " + runCmd, shell=True)
      for row in output.split(b'\n'):
        if b'real' in row:
          real, time = row.split(b' ')
          scores.append(float(time))          
    else:
      print("  run - WIN 32")
      output = subprocess.check_output("powershell -Command \"Measure-Command {" + runCmd + " | Out-Default}\" | findstr TotalSeconds" , shell=True)
      for row in output.split(b'\n'):
        if b'TotalSeconds' in row:
          totalSeconds, colon, time = output.split()
          scores.append(float(time.decode().replace(',','.')))
      
  copyScores = scores    
  copyScores.sort()
  minScore = copyScores[0]
  scoretable.append([minScore, submissionName, scores])
  if (submissionName == "canny_baseline"):
    baseline = minScore

scoretable.sort()
for score in scoretable:
    if (score[0] > 0):
      print(score, " + " + str(round((baseline / score[0] - 1) * 100, 2)) + "%")
    else:
      print(score, " + inf")
