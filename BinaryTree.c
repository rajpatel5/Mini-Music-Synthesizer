#include<stdio.h>
#include<stdlib.h>
#include"NoteSynth.c"

typedef struct BST_Node_Struct{     
    double key;
    double freq;
    int bar;
    double index;
  
	struct BST_Node_Struct *left; // A pointer to its left child
	struct BST_Node_Struct *right; // A pointer to its right child
} BST_Node;

BST_Node *newBST_Node(double freq, int bar, double index){
    /* 
     * Creates and initializes a new BST_Node 
     * for a note with the given position (bar:index) and
     * the specified frequency. The key value for the node
     * is computed here as (10.0*bar)+index.
     */
	 
	BST_Node *new_node = NULL; // Pointer to the new node
	
	new_node = (BST_Node *)calloc(1, sizeof(BST_Node));
	
	if (new_node != NULL){
		new_node->key = (10.0 * bar) + index;
		new_node->freq = freq;
		new_node->bar = bar;
		new_node->index = index;
		
		new_node->left = NULL;
		new_node->right = NULL;
	}
	
    return new_node;
}

BST_Node *BST_insert(BST_Node *root, BST_Node *new_node){
    /*
     * Inserts a new node into the BST. The
     * node must already have been initialized with valid
     * note data, and must have its unique key.
     */
	 
	if (root == NULL) return new_node; // the root 
	
	// Determine which subtree the new key should be in
	if (new_node->key != root->key){
		if (new_node->key < root->key){
			root->left = BST_insert(root->left, new_node);
		}
		else{
			root->right = BST_insert(root->right, new_node);
		}
	} 
	else{
		printf("Duplicate node requested (bar:index)=%d,%lf, it was ignored\n", root->bar, root->index);
	}

    return root;
}

BST_Node *BST_search(BST_Node *root, int bar, double index){
    /*
     * Searches the BST for a note at the
     * speficied position. If found, it returns a
     * pointer to the node that contains it.
     */
    
	double key = (10.0*bar) + index;
	 
	if (root == NULL) return NULL;
	
	if (root->key == key){
		return root;
	}
	if (key < root->key){
		return BST_search(root->left, bar, index);
	} 
	else{
		return BST_search(root->right, bar, index);
	}
	
    return NULL;
}

BST_Node *find_successor(BST_Node *right_child_node){
    /*
     * Finds the successor of a node by 
     * searching the right subtree for the node that
     * is most to the left (that will be the node
     * with the smallest key in that subtree)
     */
	
	BST_Node *p = NULL;
	p = right_child_node;
	
	while (p->left != NULL){
		p = p->left;
	}

    return p;
    
}

BST_Node *BST_delete(BST_Node *root, int bar, double index){
    /*
     * Deletes from the BST a note at the specified position.
     */
	 
	BST_Node *tmp = NULL;
	double key = (10.0*bar) + index;
	
	if (root == NULL) return NULL; // Tree or sub-tree is empty
	
	if (root->key == key){
		if (root->left == NULL && root->right == NULL) {
			// Case a), no children. 
			free(root);
			return NULL;
		}
		else if (root->right == NULL) {
			// Case b), only one child
			tmp = root->left;
			free(root);
			return tmp;
		}
		else if (root->left == NULL) {
			// Case b), only one child
			tmp = root->right;
			free(root);
			return tmp;
		}else{
			// Case c), two children. 
			tmp = find_successor(root->right);
			root->key = tmp->key;
			root->freq = tmp->freq;
			root->bar = tmp->bar;
			root->index = tmp->index;
			BST_delete(tmp, tmp->bar, tmp->index);
			return root;
		}
	}
	
	// Not in this node, delete on the corresponding subtree and
	// update the the corresponding link
	if (key < root->key){
		root->left=BST_delete(root->left, bar, index);
	}else{
		root->right=BST_delete(root->right, bar, index);
	}
	
	return root;
}

void BST_makePlayList(BST_Node *root){
    /*
     * Performs an in-order traversal of the BST to
     * generate an ordered list of notes to be played. Each
     * note is added to a linked-list and the 
     * play list is then playable using the code in NoteSynth.c
     */
	 
	if (root == NULL) return;
	
	if (root->left != NULL){
		BST_makePlayList(root->left);
	}
	
	playlist_head = playlist_insert(playlist_head, root->freq, root->bar, root->index);
	
	if (root->right != NULL){
		BST_makePlayList(root->right);
	}
	
}

void BST_inOrder(BST_Node *root, int depth){
    /*
     * Performs an in-order traversal of the BST
     * and prints out the note information for each note
     * using this print statement:
     * 
     *  printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n",...);
     */
    
	if (root == NULL) return;
	
	if (root->left != NULL){
		BST_inOrder(root->left, depth + 1);
	}
	
	printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth, root->bar, root->index, root->freq);
	
	if (root->right != NULL){
		BST_inOrder(root->right, depth + 1);
	}
} 

void BST_preOrder(BST_Node *root, int depth){
    /*
     * Performs an pre-order traversal of the BST
     * and prints out the note information for each note
     * using this print statement:
     * 
     *  printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n",...);
     */
	
	if (root == NULL) return;
	
	printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth, root->bar, root->index, root->freq);
	
	if (root->left != NULL){
		BST_preOrder(root->left, depth + 1);
	}
	
	if (root->right != NULL){
		BST_preOrder(root->right, depth + 1);
	}
}

void BST_postOrder(BST_Node *root,int depth){
    /*
     * Performs an post-order traversal of the BST
     * and prints out the note information for each note
     * using this print statement:
     * 
     *  printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n",...);
     */
	 
	if (root == NULL) return;
	
	if (root->left != NULL){
		BST_postOrder(root->left, depth + 1);
	}
	
	if (root->right != NULL){
		BST_postOrder(root->right, depth + 1);
	}
	
	printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth, root->bar, root->index, root->freq);
} 

void delete_BST(BST_Node *root){
    /*
     * Deletes the BST and frees all memory used for
     * nodes in it. Recall that there is a specific order in which
     * this needs to be done! (consult the Unit 4 notes as needed)
     */
	
	if (root->left != NULL){
		delete_BST(root->left);
	}
	
	if (root->right != NULL){
		delete_BST(root->right);
	}
	
	free(root);
}

void replace(BST_Node *root, int pos_src, int pos_dst){
	if (root == NULL) return;
	
	if (root->left != NULL){
		replace(root->left, pos_src, pos_dst);
	}
	
	if (root->freq == note_freq[pos_src]){
		root->freq = note_freq[pos_dst];
	}
	
	if (root->right != NULL){
		replace(root->right, pos_src, pos_dst);
	}
}

void BST_shiftFreq(BST_Node *root, char note_src[5], char note_dst[5]){
    /*
     * Goes over the entries in the BST, finds notes
     * corresponding to note_src (which is a text string describing
     * a note's name in the musical scale), and shifts those notes
     * to the frequency of note_dst (another text string with a note
     * name). Effectively, it replaces one particular type of note
     * by another all through the song.
     * 
     * The note names are in either 2 or 3 characters long, and
     * you can see all of them in the file 'note_frequencies.txt'
     * 
     * Expected result: All notes in the BST whose frequency
     * corresponds to the frequency of note_src, should have
     * their frequency changed to that of note_dst.
     */
	 
	int pos_src, pos_dst;
	
	if (strlen(note_src) <= 3 && strlen(note_dst) <= 3){
		for (int i = 0; i < 100; i++){
			if (strcmp(note_names[i], note_dst) == 0){
				pos_dst = i;
			}
			if (strcmp(note_names[i], note_src) == 0){
				pos_src = i;
			}
		}
		
		replace(root, pos_src, pos_dst);
	}
}

void insert_harmonized_notes(BST_Node *harmonize_root, BST_Node *head_root){
	BST_Node *insertNote = NULL;
	
	if (harmonize_root->left != NULL){
		insert_harmonized_notes(harmonize_root->left, head_root);
	}
	
	insertNote = newBST_Node(harmonize_root->freq, harmonize_root->bar, harmonize_root->index);
	BST_insert(head_root, insertNote);
	
	if (harmonize_root->right != NULL){
		insert_harmonized_notes(harmonize_root->right, head_root);
	}
}

BST_Node *harmonize_node(BST_Node* root, BST_Node *head_root, BST_Node *harmonize_head_root, int semitones, double time_shift){
	for (int i = 0; i < 100; i++){
		if (note_freq[i] == root->freq && (i + semitones >= 0 && i + semitones <= 99)){
			
			BST_Node *harmony_node = newBST_Node(note_freq[i + semitones], root->bar,\
			root->index + time_shift);
			
			while (BST_search(head_root, harmony_node->bar, harmony_node->index) != NULL ||\
			BST_search(harmonize_head_root, harmony_node->bar, harmony_node->index) != NULL){
				harmony_node->index = harmony_node->index + 0.000001;
				harmony_node->key = harmony_node->key + 0.000001;
			}
			
			return newBST_Node(harmony_node->freq, harmony_node->bar, harmony_node->index);
		}
	}
	
	return NULL;
}

BST_Node *generate_harmonize_BST(BST_Node *root, BST_Node *head_root, BST_Node *harmonize_head_root,\
int semitones, double time_shift){

	if (root == NULL) return NULL;
	
	BST_Node *harmonized_node = NULL;
	
	if (root->left != NULL){
		harmonize_head_root = generate_harmonize_BST(root->left, head_root, harmonize_head_root, semitones, time_shift);
	}
	
	if (root->right != NULL){
		harmonize_head_root = generate_harmonize_BST(root->right, head_root, harmonize_head_root, semitones, time_shift);
	}
	
	if (root->index + time_shift >= 0.0 && root->index + time_shift < 1.0){
		harmonized_node = harmonize_node(root, head_root, harmonize_head_root, semitones, time_shift);
	}
	
	return BST_insert(harmonize_head_root, harmonized_node);
	
}

BST_Node *BST_harmonize(BST_Node *root, int semitones, double time_shift){
    /* 
     * Traverses the BST, and for each existing
     * note, inserts a new, modified note (i.e. it will add sounds
     * to an already existing song, based on the notes it already has)
     * 
     * Expected result: The BST will have about twice as many notes
     *   as before, and the new notes are shifted in pitch and 
     *   in time as specified by the parameters. 
     */
	
	if (root != NULL){
		BST_Node *harmonize_root = NULL;
		
		harmonize_root = generate_harmonize_BST(root, root, harmonize_root, semitones, time_shift);
		insert_harmonized_notes(harmonize_root, root);
		delete_BST(harmonize_root);
		
		harmonize_root = NULL;
	}
	
    return root;

}
