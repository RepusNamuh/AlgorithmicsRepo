def merge_sort(num_list):
    if len(num_list) <= 1:
        return num_list
    
    median = len(num_list) // 2
    Left = merge_sort(num_list[:median])
    Right = merge_sort(num_list[median:])
    return merge(Left, Right)
    
def merge(left, right):
    output = []
    while len(left) != 0 or len(right) != 0:
        if len(left) == 0 or len(right) == 0:
            output += right + left
            break
        else:
            if left[0] < right[0]:
                output.append(left[0])
                left.pop(0)
            elif right[0] < left[0]:
                output.append(right[0])
                right.pop(0)
            else:
                output.append(left[0])
                output.append(right[0])
                left.pop(0)
                right.pop(0)

    return output
