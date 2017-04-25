//
//  ItemsListViewController.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/25/17.
//  Copyright © 2017 acm. All rights reserved.
//

import UIKit
import GrootSwift

class ItemsListViewController: UIViewController, UITableViewDelegate {
    
    // MARK: - Outlets
    @IBOutlet weak var tableView: UITableView!
    
    // MARK: - Variables
    let refreshControl = UIRefreshControl()

    // MARK: - UIViewController
    override func viewDidLoad() {
        super.viewDidLoad()
        tableView.dataSource = ItemsDataSource.shared
        tableView.delegate = self
        
        refreshControl.tintColor = UIConstants.Colors.secondary
        refreshControl.addTarget(self, action: #selector(refresh(_:)), for: UIControlEvents.valueChanged)
        
        tableView.refreshControl = refreshControl
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        refresh(nil)
    }
    
    // MARK: - Actions

    // MARK: - UITableViewDelegate
    func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
        return 105
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        tableView.deselectRow(at: indexPath, animated: false)
        
        (parent as? ItemsViewController)?.didSelect(item: ItemsDataSource.shared.items[indexPath.row])
    }
    
    // MARK: - Refresh Items
    func refresh(_ sender: Any?) {
        (sender as? UIRefreshControl)?.beginRefreshing()
        
        ItemsDataSource.shared.clear()
        tableView.reloadData()
        
        if let user = UserModel.shared {
            GrootMerchService.getItems()
            .onSuccess { (json) in
                DispatchQueue.main.async {
                    if let models = json["data"] as? [[String: Any]] {
                        ItemsDataSource.shared.populate(models: models)
                    }
                    self.tableView.reloadData()
                    (sender as? UIRefreshControl)?.endRefreshing()
                }
            }
            .onFailure{ (error) in
                print(error)
                DispatchQueue.main.async {
                    self.tableView.reloadData()
                    (sender as? UIRefreshControl)?.endRefreshing()
                }
            }.perform(withAuthorization: user)
        } else {
            (sender as? UIRefreshControl)?.endRefreshing()
        }
    }
}
