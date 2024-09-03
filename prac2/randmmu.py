from mmu import MMU
import random

class RandMMU(MMU):
    def __init__(self, frames):
        self.frames = frames
        self.page_table = {}  # {page_number: (frame_index, modified_bit)}
        self.frame_list = []
        self.total_disk_reads = 0
        self.total_disk_writes = 0
        self.total_page_faults = 0
        self.debug = False

    def set_debug(self):
        self.debug = True

    def reset_debug(self):
        self.debug = False

    def read_memory(self, page_number):
        if page_number not in self.page_table:
            self.handle_page_fault(page_number, False)
        # No need to update if it's a read and already in memory

    def write_memory(self, page_number):
        if page_number not in self.page_table:
            self.handle_page_fault(page_number, True)
        else:
            frame_index, _ = self.page_table[page_number]
            self.page_table[page_number] = (frame_index, 1)  # Set modified bit

    def handle_page_fault(self, page_number, is_write):
        self.total_page_faults += 1
        self.total_disk_reads += 1

       
