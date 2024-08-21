document.addEventListener("DOMContentLoaded", function() {
    const searchInput = document.getElementById("search-input");
    let timeout;
    // 监听输入框的变化
    searchInput.addEventListener("input", function() {
       clearTimeout(timeout); 
       const query = searchInput.value;

        // 发送请求到后端
        fetch("http://192.168.220.128/search", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({ query: query })
        })
        .then(response => {
            if (!response.ok) {
                throw new Error("网络错误");
            }
            return response.json();
        })
        .then(data => {
            console.log("响应数据:", data);
            // 处理响应数据
        })
        .catch(error => {
            console.error("请求失败:", error);
        });
    }，300);
});

