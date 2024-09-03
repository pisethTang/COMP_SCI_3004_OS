from mmu import MMU

class ClockMMU(MMU):
    def __init__(self, frames):
        self.frames = frames
        self.page_table = {}  # {page_number: [frame_index, modified_bit, use_bit]}
        self.frame_list = []  # [(page_number, modified_bit, use_bit)]
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
            # Page is already in memory, just update the use bit
            frame_index, modified_bit, use_bit = self.page_table[page_number]
            self.page_table[page_number] = [frame_index, modified_bit, 1]  # Set use bit to 1

    def write_memory(self, page_number):
        if page_number not in self.page_table:
            self.handle_page_fault(page_number, True)
        else:
            # Page is already in memory, update both modified and use bits
            frame_index, modified_bit, use_bit = self.page_table[page_number]
            self.page_table[page_number] = [frame_index, 1, 1]  # Set modified and use bits to 1

    def handle_page_fault(self, page_number, is_write):
        self.total_page_faults += 1
        self.total_disk_reads += 1  # We read from disk only when there is a page fault

        if len(self.frame_list) < self.frames:
            # There is still space in the frame list, add new page directly
            frame_index = len(self.frame_list)
            self.page_table[page_number] = [frame_index, int(is_write), 1]
            self.frame_list.append((page_number, int(is_write), 1))
        else:
            # No space left, need to find a page to replace using the Clock algorithm
            while True:
                current_page, modified_bit, use_bit = self.frame_list[self.clock_hand]

                if use_bit == 0:
                    # Found a page to replace
                    if modified_bit == 1:
                        self.total_disk_writes += 1  # Only increment disk writes if the page was modified

                    # Replace the current page with the new page
                    self.page_table.pop(current_page)
                    self.page_table[page_number] = [self.clock_hand, int(is_write), 1]
                    self.frame_list[self.clock_hand] = (page_number, int(is_write), 1)

                    # Advance the clock hand
                    self.clock_hand = (self.clock_hand + 1) % self.frames
                    break
                else:
                    # Reset the use bit and advance the clock hand
                    self.frame_list[self.clock_hand] = (current_page, modified_bit, 0)
                    self.clock_hand = (self.clock_hand + 1) % self.frames

    def get_total_disk_reads(self):
        return self.total_disk_reads

    def get_total_disk_writes(self):
        return self.total_disk_writes

    def get_total_page_faults(self):
        return self.total_page_faults
