# from mmu import MMU
# import random

# class RandMMU(MMU):
#     def __init__(self, frames):
#         self.frames = frames
#         self.page_table = {}  # {page_number: (frame_index, modified_bit)}
#         self.frame_list = []
#         self.total_disk_reads = 0
#         self.total_disk_writes = 0
#         self.total_page_faults = 0
#         self.debug = False

#     def set_debug(self):
#         self.debug = True

#     def reset_debug(self):
#         self.debug = False

#     def read_memory(self, page_number):
#         if page_number not in self.page_table:
#             self.handle_page_fault(page_number, False)
#         # No need to update if it's a read and already in memory

#     def write_memory(self, page_number):
#         if page_number not in self.page_table:
#             self.handle_page_fault(page_number, True)
#         else:
#             frame_index, _ = self.page_table[page_number]
#             self.page_table[page_number] = (frame_index, 1)  # Set modified bit

#     def handle_page_fault(self, page_number, is_write):
#         self.total_page_faults += 1
#         self.total_disk_reads += 1

       
# randmmu.py

from mmu import MMU
import random


class RandMMU(MMU):
    def __init__(self, frames):
        super().__init__(frames)
        self.frames = frames
        self.page_table = {}  # Stores page_number -> (frame_number, modified_bit)
        self.frame_table = [-1] * frames  # Stores frame_number -> page_number
        self.disk_reads = 0
        self.disk_writes = 0
        self.page_faults = 0
        self.debug = False

    def set_debug(self):
        self.debug = True

    def reset_debug(self):
        self.debug = False

    def read_memory(self, page_number):
        if self.debug:
            print(f"Reading page {page_number}")
        self.access_memory(page_number, is_write=False)

    def write_memory(self, page_number):
        if self.debug:
            print(f"Writing to page {page_number}")
        self.access_memory(page_number, is_write=True)

    def access_memory(self, page_number, is_write):
        # Check if page is already in memory
        if page_number in self.page_table:
            frame_number, modified = self.page_table[page_number]
            self.page_table[page_number] = (frame_number, 1 if is_write else modified)
        else:
            self.page_faults += 1
            self.disk_reads += 1
            # Check for available frame
            available_frame = self.find_available_frame()
            if available_frame is None:
                # No free frame; choose a random page to replace
                available_frame = self.replace_page()
            # Load the new page into the available frame
            self.frame_table[available_frame] = page_number
            self.page_table[page_number] = (available_frame, 1 if is_write else 0)

    def find_available_frame(self):
        for frame in range(self.frames):
            if self.frame_table[frame] == -1:
                return frame
        return None

    def replace_page(self):
        victim_frame = random.choice(range(self.frames))
        victim_page = self.frame_table[victim_frame]
        _, modified = self.page_table[victim_page]
        if modified:
            self.disk_writes += 1
        del self.page_table[victim_page]
        return victim_frame

    def get_total_disk_reads(self):
        return self.disk_reads

    def get_total_disk_writes(self):
        return self.disk_writes

    def get_total_page_faults(self):
        return self.page_faults
