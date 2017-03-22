//
//  ItemsViewController.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/21/17.
//  Copyright © 2017 acm. All rights reserved.
//

import UIKit

class ItemsViewController: UIViewController, UITableViewDelegate {

    // MARK: - Outlets
    @IBOutlet weak var timerLabel: UILabel!
    @IBOutlet weak var nameLabel: UILabel!
    @IBOutlet weak var balanceLabel: UILabel!
    
    @IBOutlet weak var tableView: UITableView!
    
    // MARK: - Variables
    let updateItems = APIRequest.getItems(success: { (json) in
        ItemsDataSource.shared.clear()
        ItemsDataSource.shared.populate(models: json)
    }) { (error) in
        print(error)
    }
    
    // MARK: - UIViewController
    override func viewDidLoad() {
        super.viewDidLoad()
        tableView.dataSource = ItemsDataSource.shared
        tableView.delegate = self
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
        if let user = UserModel.shared {
            APIManager.performRequest(request: updateItems, withAuthorization: user)
        }
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        
        if UserModel.shared == nil {
            let alert = UIAlertController(title: "Internal Error", message: "Unable to load user", preferredStyle: .alert)
            let okAction = UIAlertAction(title: "Ok", style: .default, handler: { (_) in
                self.logout()
            })
            alert.addAction(okAction)
            present(alert, animated: true, completion: nil)
        }
    }

    // MARK: - Actions
    @IBAction func logout() {
        dismiss(animated: true, completion: nil)
    }
    
    // MARK: - UITableViewDelegate
    func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
        return 105
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        tableView.deselectRow(at: indexPath, animated: false)
    }
}
