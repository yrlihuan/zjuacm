import sys

def calc_area(x1, y1, x2, y2):
  w = max(x2 - x1, 0)
  h = max(y2 - y1, 0)

  return w * h

def calc_iou(obj1, obj2):
  x1 = max(obj1[0], obj2[0])
  y1 = max(obj1[1], obj2[1])
  x2 = min(obj1[2], obj2[2])
  y2 = min(obj1[3], obj2[3])

  w = max(x2 - x1, 0)
  h = max(y2 - y1, 0)
  inter = w * h

  area1 = calc_area(*obj1[:4])
  area2 = calc_area(*obj2[:4])
  inter = calc_area(x1, y1, x2, y2)

  iou = inter / (area1 + area2 - inter)
  return iou

def solve_nms(objects, th):
  objects.sort(key=lambda t: -t[-2])
  ret = []

  for obj in objects:
    overlap = False
    for valid_obj in ret:
      if calc_iou(valid_obj, obj) > th:
        overlap = True
        break

    if not overlap:
      ret.append(obj)

  return ret

if __name__ == '__main__':
  infile = sys.argv[1]

  objects = []
  with open(infile) as fin:
    l = fin.readline()

    n, m, th = l.split(' ')
    n = int(n)
    m = int(m)
    th = float(th)

    for i in range(n):
      l = fin.readline()
      x1, y1, x2, y2, conf, t = l.split(' ')
      objects.append((int(x1), int(y1), int(x2), int(y2), float(conf), int(t)))

  remained_objects = []
  for target_t in range(m):
    type_objects = list(filter(lambda t: t[-1] == target_t, objects))
    remained_objects += solve_nms(type_objects, th)

  print(len(remained_objects))
  remained_objects.sort()
  for obj in remained_objects:
    print(*obj[:4])


