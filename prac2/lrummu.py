from mmu import MMU
from collections import OrderedDict

class LruMMU(MMU):
    def __init__(self, frames):
        self.frames = frames
        self.page_table = OrderedDict()  # {page_number: (frame_index, modified_bit)}
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
        else:
            frame_index, modified_bit = self.page_table.pop(page_number)
            self.page_table[page_number] = (frame_index, modified_bit)  # Re-insert as most recently used

    def write_memory(self, page_number):
        if page_number not in self.page_table:
            self.handle_page_fault(page_number, True)
        else:
            frame_index, _ = self.page_table.pop(page_number)
            self.page_table[page_number] = (frame_index, 1)  # Set modified bit and re-insert

    def handle_page_fault(self, page_number, is_write):
        self.total_page_faults += 1
        self.total_disk_reads += 1

        if len(self.page_table) >= self.frames:
            oldest_page, (frame_index, modified_bit) = self.page_table.popitem(last=False)
            if modified_bit == 1:
                self.total_disk_writes += 1

        else:
            frame_index = len(self.page_table)

        self.page_table[page_number] = (frame_index, int(is_write))

    def get_total_disk_reads(self):
        return self.total_disk_reads

    def get_total_disk_writes(self):
        return self.total_disk_writes

    def get_total_page_faults(self):
        return self.total_page_faults
