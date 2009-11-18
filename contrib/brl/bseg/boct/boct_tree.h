//This is brl/bseg/boct/boc_tree.h
#ifndef boct_tree_h_
#define boct_tree_h_
//:
// \file
// \brief  A generic octree templated on locational code and datatype. Travesal operations are inplemented as described in 
//         [Simple and Efficient Travessal Methods for Quadtrees and Octrees/ Frisken, Perry 2002]
//
// \author Gamze Tunali/Vishal Jain
// \date   July 31, 2009
// \verbatim
//  Modifications
//   <none yet>
// \endverbatim

#include "boct_tree_cell.h"
#include "boct_loc_code.h"

#include <vgl/vgl_point_3d.h>
#include <vsl/vsl_binary_io.h>
#include <vgl/vgl_box_3d.h>

template <class T_loc, class T_data>
class boct_tree
{
 public:
  //: Default constructor
  boct_tree(): root_(0),root_level_(-1),num_levels_(0), max_val_(0), global_bbox_(){}
  
  //: Construct an empty tree from maximum number of levels and levels, to initialize
  boct_tree(short num_levels, short init_levels=1);
  
  //: Construct from bounding box, maximum number of levels and levels to initialize
  boct_tree(vgl_box_3d<double>  bbox,short num_levels, short init_levels=1);
  
  //: Constructs form root ceel and maximum number of levels
  boct_tree(boct_tree_cell<T_loc, T_data>* root, short num_levels)
  {
    root_ = root;
    root_level_ = num_levels -1;
    num_levels_ = num_levels;
    max_val_ = (double)(1 << root_level_);
  }
  
  //: Destructor
  ~boct_tree();

  //: Clones with the same data
  boct_tree<T_loc,T_data>* clone();

  //: Dlones the tree structure with a different data type
  template <class T_data_to>
  boct_tree<T_loc,T_data_to>* clone_to_type() {
    // clone the tree
    boct_tree_cell<T_loc, T_data_to>* root = root_->clone_to_type<T_data_to>(0);
    // create a new tree tree only with the root node
    boct_tree<T_loc,T_data_to>* tree = new boct_tree<T_loc,T_data_to>(root,num_levels_);
  
    return tree;
  }

  //: Initialize all cells with a value
  void init_cells(T_data val);
  
  //: Returns the leaf cell that contains the 3d point specified in global coordinates
  boct_tree_cell<T_loc,T_data>* locate_point_global(const vgl_point_3d<double>& p);
  
  //: Returns the leaf cell that contains the 3d point specified in octree-coordinates i.e. [0,1)x[0,1)x[0,1), with optional safe check for out of bounds
  boct_tree_cell<T_loc,T_data>* locate_point(const vgl_point_3d<double>& p, bool check_out_of_bounds = false);
  
  //: Returns the cell at a particular level(not necessarly a leaf) containing teh 3d point in octree-coordinates i.e. [0,1)x[0,1)x[0,1),with optional safe check for out of bounds
  boct_tree_cell<T_loc,T_data>* locate_point_at_level(const vgl_point_3d<double>& p, short level, bool check_out_of_bounds = false);

  //: Returns the smallest cell that entirely contains a 3d region in octree coordinates [0,1)x[0,1)x[0,1)
  boct_tree_cell<T_loc,T_data>* locate_region(const vgl_box_3d<double>& r);
  
  //: Return cell with a partcular locational code
  boct_tree_cell<T_loc,T_data>* get_cell(  boct_loc_code<T_loc>& code) { return root_->traverse(code); }
  
  //: Return root of the tree
  boct_tree_cell<T_loc,T_data>* root(){return root_;}
  
  //: Split the tree, this only splits the root cell
  bool split();
  
  //: Returns a vector of all leaf cells of the tree
  vcl_vector<boct_tree_cell<T_loc,T_data>*> leaf_cells();
  
  //: Returns all leaf cells at a specified level of the tree
  vcl_vector<boct_tree_cell<T_loc,T_data>*> leaf_cells_at_level(short l);
  
  //: Returns all cells at a specified level of the tree (wheather or not they are leafs)
  vcl_vector<boct_tree_cell<T_loc,T_data>*> cells_at_level(short l);
  
  //: Return the maximum number of levels, which is root_level+1
  short number_levels() const { return num_levels_; }
  
  //: Return the levels of the root, which is number_levels-1
  short root_level() const { return root_level_; }
  
  //: Return a value for converting positions to locational codes 2^(root_level)
  double max_val() const {return max_val_;}
 
  //: Return the finest level the tree has been split down to (not necessarly 0)
  short finest_level();

  //: Set the global-coordinates bounding box for the tree
  void set_bbox(vgl_box_3d<double> & bbox){global_bbox_=bbox;}

  //: Return the bounding box of a cell
  vgl_box_3d<double> cell_bounding_box(boct_tree_cell<T_loc,T_data>* const cell);

  //: Returns the location of the cell in the block, with respect to blocks min point
  vgl_box_3d<double> cell_bounding_box_local(boct_tree_cell<T_loc,T_data>* const cell);

  //: Returns bounding box of the tree
  vgl_box_3d<double> bounding_box() const {return global_bbox_;}

  //: Returns the length of a cell, assuming cell is cubical
  double cell_size(boct_tree_cell<T_loc,T_data>* const cell)
  {
    return 1.0/(double)(1<<(root_level_-cell->level())); 
  }
  
  //: Return cell's local origin
  vgl_point_3d<double> local_origin(boct_tree_cell<T_loc,T_data>* const cell)
  {
    return vgl_point_3d<double> (cell->code_.x_loc_/max_val_,cell->code_.y_loc_/max_val_,cell->code_.z_loc_/max_val_);
  }

  
  //: Print tree
  void print();

  // Binary I/O
  void b_write(vsl_b_ostream & os);
  void b_read(vsl_b_istream & is);
  static short version_no() { return 2; }
  
 private:
  //: Maximum number of levels in the octree
  short num_levels_;
  
  //: Level of the root cell i.e root_level = num_level - 1
  short root_level_;
  
  //: Value for converting positions to locational codes 2^(root_level)
  double max_val_;
  
  //: The root cell
  boct_tree_cell<T_loc,T_data>* root_;

  //: The local coordinates are from 0 to 1; that's why we need the global coordinates
  vgl_box_3d<double> global_bbox_;
  
public: //should be made private
  //: Constructs the tree from leaf nodes and number of levels, returns the root cell. This function is used by b_read
  boct_tree_cell<T_loc, T_data>* construct_tree(vcl_vector<boct_tree_cell<T_loc, T_data> >& leaf_nodes, short num_levels);
};

//: Binary write
template <class T_loc,class T_data>
void vsl_b_write(vsl_b_ostream & os, boct_tree<T_loc,T_data>& tree) { tree.b_write(os); }

//: Binary read
template <class T_loc,class T_data>
void vsl_b_read(vsl_b_istream & is, boct_tree<T_loc,T_data>& tree) { tree.b_read(is); }

#endif
