from mmu import MMU

class ClockMMU(MMU):
    def __init__(self, frames):
        self.frames = frames
        self.page_table = {}  # {page_number: [frame_index, modified_bit, reference_bit]}
        self.frame_list = []  # [(page_number, modified_bit, reference_bit)]
        self.clock_hand = 0
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
            frame_index, modified_bit, reference_bit = self.page_table[page_number]
            self.page_table[page_number] = [frame_index, modified_bit, 1]  # Set reference bit

    def write_memory(self, page_number):
        if page_number not in self.page_table:
            self.handle_page_fault(page_number, True)
        else:
            frame_index, modified_bit, reference_bit = self.page_table[page_number]
            self.page_table[page_number] = [frame_index, 1, 1]  # Set modified and reference bits

    def handle_page_fault(self, page_number, is_write):
        self.total_page_faults += 1
        self.total_disk_reads += 1

        if len(self.frame_list) < self.frames:
            # Add the new page to a free frame
            frame_index = len(self.frame_list)
            self.page_table[page_number] = [frame_index, int(is_write), 1]
            self.frame_list.append((page_number, int(is_write), 1))
        else:
            # Page replacement using the Clock algorithm
            while True:
                current_page, modified_bit, reference_bit = self.frame_list[self.clock_hand]
                
                if reference_bit == 0:
                    # Evict this page
                    if modified_bit == 1:
                        self.total_disk_writes += 1
                    self.page_table.pop(current_page)
                    self.page_table[page_number] = [self.clock_hand, int(is_write), 1]
                    self.frame_list[self.clock_hand] = (page_number, int(is_write), 1)
                    self.clock_hand = (self.clock_hand + 1) % self.frames
                    break
                else:
                    # Reset the reference bit
                    self.frame_list[self.clock_hand] = (current_page, modified_bit, 0)
                    self.clock_hand = (self.clock_hand + 1) % self.frames

    def get_total_disk_reads(self):
        return self.total_disk_reads

    def get_total_disk_writes(self):
        return self.total_disk_writes

    def get_total_page_faults(self):
        return self.total_page_faults
