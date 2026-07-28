"""
Description:
Randomly select ep DIS events according to a nuclear modification factor.

"""
##################Params####################

output_dir = "output/output_batch_dis_ep_NC_18x110"

############################################
import pyhepmc
import os
import shutil
import numpy as np

def nuclear_modification_factor(x, Q2):
    # Example nuclear modification factor function
    # This is a placeholder; replace with your actual function
    return 0.5 * np.log(x)

def decide_event(event):
    # Extract relevant kinematic variables from the event
    if event.pdf_info is None:
        raise ValueError("Event does not contain PDF information.")
    event_x = event.pdf_info.x1 #I guess this is the correct x? still double check
    event_Q2 = event.pdf_info.scale ** 2

    print(f"Event x: {event_x}, Q2: {event_Q2}")

    # Calculate the nuclear modification factor
    keep_probability = nuclear_modification_factor(event_x, event_Q2)

    #random number between 0 and 1
    random_number = np.random.rand()
    if random_number > keep_probability:
        return False
    return True

def reweight_hepmc(hepmc_file):
    input_file = os.path.join(hepmc_input, hepmc_file)
    output_file = os.path.join(hepmc_output, hepmc_file)
    with pyhepmc.open(input_file) as f_in, pyhepmc.open(output_file, "w") as f_out:
        for event in f_in:
            if decide_event(event):
                f_out.write(event)


hepmc_input = os.path.join(output_dir, "hepmc")
hepmc_output = os.path.join(output_dir, "hepmc_reweighted")
if not os.path.exists(hepmc_output):
    os.makedirs(hepmc_output)
else:
    shutil.rmtree(hepmc_output)
    os.makedirs(hepmc_output)

#get .hepmc files in hepmc_input
hepmc_files = [f for f in os.listdir(hepmc_input) if f.endswith(".hepmc")]
for hepmc_file in hepmc_files:
    reweight_hepmc(hepmc_file)
