"""	 Self Balancing BinarySearchTree - Adelson-Velsky and Landis Tree Implementation	"""


class TreeNode:
	def __init__(self, data, left = None, right = None, parent = None):
		self.data = data
		self.height = 0
		self.left_child = left
		self.right_child = right
		self.parent = parent

	def has_left_child(self):
		return self.left_child

	def has_right_child(self):
		return self.right_child

	def is_left_child(self):
		return self.parent and self.parent.left_child == self

	def is_right_child(self):
		return self.parent and self.parent.right_child == self

	def is_root(self):
		return not self.parent

	def is_leaf(self):
		return not (self.right_child or self.left_child)

	def has_any_children(self):
		return self.right_child or self.left_child

	def has_both_children(self):
		return self.right_child and self.left_child

	def replace_node_data(self, data, lc, rc):
		self.data = data
		self.left_child = lc
		self.right_child = rc
		if self.has_left_child():
			self.left_child.parent = self
		if self.has_right_child():
			self.right_child.parent = self

	def _delete(self,data,found):
		if self == None:
			return

		if data < self.data:
			if self.left_child:
				self.left_child = self.left_child._delete(data,found)
		elif data > self.data:
			if self.right_child:
				self.right_child = self.right_child._delete(data,found)
		else:
			found[0] = 1
			if self.is_leaf(): #leaf
				if self == self.parent.left_child:
					self.parent.left_child = None
				else:
					self.parent.right_child = None
				return

			elif self.has_both_children(): # interior
				succ = self.find_successor()
				succ.splice_out()
				self.data = succ.data

			else: # has only one child
				if self.has_left_child():
					if self.is_left_child():
						self.left_child.parent = self.parent
						self.parent.left_child = self.left_child
					elif self.is_right_child():
						self.left_child.parent = self.parent
						self.parent.right_child = self.left_child
					else:
						self.replace_node_data(self.left_child.data,
						self.left_child.left_child,
						self.left_child.right_child)
				else:
					if self.is_left_child():
						self.right_child.parent = self.parent
						self.parent.left_child = self.right_child
					elif self.is_right_child():
						self.right_child.parent = self.parent
						self.parent.right_child = self.right_child
					else:
						self.replace_node_data(self.right_child.data,
						self.right_child.left_child,
						self.right_child.right_child)

		self.height = max(findHeight(self.left_child),findHeight(self.right_child)) + 1

		hFactor = checkHeight(self)

		if hFactor > 1 and findHeight(self.left_child) >= 0:
			return self.rightRotate()

		if hFactor > 1 and findHeight(self.left_child) < 0:
			self.left_child = self.left_child.leftRotate()
			return self.rightRotate()

		if hFactor < -1 and findHeight(self.right_child) <= 0:
			return self.leftRotate()

		if hFactor < -1 and findHeight(self.right_child) > 0:
			self.right_child = self.right_child.rightRotate()
			return self.leftRotate()

		return self

	def splice_out(self):
		if self.is_leaf():
			if self.is_left_child():
				self.parent.left_child = None
			else:
				self.parent.right_child = None
		elif self.has_any_children():
			if self.has_left_child():
				if self.is_left_child():
					self.parent.left_child = self.left_child
				else:
					self.parent.right_child = self.left_child
					self.left_child.parent = self.parent
			else:
				if self.is_left_child():
					self.parent.left_child = self.right_child
				else:
					self.parent.right_child = self.right_child
					self.right_child.parent = self.parent

	def find_successor(self):
		succ = None
		if self.has_right_child():
			succ = self.right_child.find_min()
		else:
			if self.parent:
				if self.is_left_child():
					succ = self.parent
				else:
					self.parent.right_child = None
					succ = self.parent.find_successor()
					self.parent.right_child = self
		return succ

	def find_min(self):
		current = self
		while current.has_left_child():
			current = current.left_child
		return current

	def leftRotate(self):
		tmp = self.right_child
		self.right_child = tmp.left_child
		tmp.left_child = self

		if self.is_root():
			tmp.parent = None
			self.parent = tmp
		else:
			tmp.parent = self.parent
			self.parent = tmp

		self.height = max(findHeight(self.left_child),findHeight(self.right_child)) + 1
		tmp.height = max(findHeight(tmp.left_child),findHeight(tmp.right_child)) + 1
		return tmp

	def rightRotate(self):
		tmp = self.left_child
		self.left_child = tmp.right_child
		tmp.right_child = self

		if self.is_root():
			tmp.parent = None
			self.parent = tmp
		else:
			tmp.parent = self.parent
			self.parent = tmp

		self.height = max(findHeight(self.left_child),findHeight(self.right_child)) + 1
		tmp.height = max(findHeight(tmp.left_child),findHeight(tmp.right_child)) + 1
		return tmp

	def _printTree(self):
		if self.has_left_child():
			self.left_child._printTree()
		print self.data,
		if self.has_right_child():
			self.right_child._printTree()



class AVLtree:
	def __init__(self):
		self.root = None
		self.size = 0

	def length(self):
		return self.size

	def __len__(self):
		return self.size

	def insert(self, data):
		if self.root:
			self.root = self._put(data, self.root)
		else:
			self.root = TreeNode(data)
			self.size = self.size + 1

	def _put(self, data, current_node):
		if data < current_node.data:
			if current_node.has_left_child():
				current_node.left_child = self._put(data, current_node.left_child)
			else:
				current_node.left_child = TreeNode(data, parent = current_node)
		else:
			if current_node.has_right_child():
				current_node.right_child = self._put(data, current_node.right_child)
			else:
				current_node.right_child = TreeNode(data, parent = current_node)

		current_node.height = max(findHeight(current_node.left_child),findHeight(current_node.right_child)) + 1

		hFactor = checkHeight(current_node)

		if hFactor > 1 and data < current_node.left_child.data:
			return current_node.rightRotate()

		if hFactor > 1 and data > current_node.left_child.data:
			current_node.left_child = current_node.left_child.leftRotate()
			return current_node.rightRotate()

		if hFactor < -1 and data > current_node.right_child.data:
			return current_node.leftRotate()

		if hFactor < -1 and data < current_node.right_child.data:
			current_node.right_child = current_node.right_child.rightRotate()
			return current_node.leftRotate()

		return current_node

	def delete(self,data,found):
		if self.root == None:
			raise KeyError('Error : Tree empty')
		else:
			self.root = self.root._delete(data,found)

	def printTree(self):
		if self.root:
			self.root._printTree()
			print
		else:
			print 'Tree empty'

def findHeight(node):
	if node:
		return node.height
	else:
		return 0

def checkHeight(node):
	if node:
		return findHeight(node.left_child) - findHeight(node.right_child)
	else:
		return 0

def main():
	t = AVLtree()
	t.insert(8)
	t.insert(3)
	t.insert(7)
	t.insert(5)
	t.insert(1)
	t.insert(9)
	t.insert(4)

	t.printTree()
	try:
		data_to_delete = int(raw_input('\n\tenter the data to delete : '))
	except ValueError:
		print '\n\n\tenter a valid number!!!\n\n'
		sys.exit(0)

	found = [None]
	t.delete(data_to_delete,found)

	if found[0]:
		print '\n\nafter deleting ',data_to_delete,'\n\n\t',
		t.printTree()
	else:
		print '\n\n\terror : %d not found!!!'%data_to_delete
	print '\n'

if __name__ == '__main__':
	import sys
	main()
