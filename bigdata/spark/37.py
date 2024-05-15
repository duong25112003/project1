import pandas as pd

# Đọc dữ liệu từ các file CSV vào DataFrame
phim_thongtin = pd.read_csv('phim.thongtin.csv')
phim_binhchon = pd.read_csv('phim.binhchon.csv')

# Kết hợp hai DataFrame dựa trên cột phim_id
merged_df = pd.merge(phim_thongtin, phim_binhchon, on='phim_id')
# Lọc các phim thỏa mãn điều kiện
filtered_df = merged_df[(merged_df['luot_binh_chon'] > 100000) & (merged_df['nam_bat_dau'] > 2000)]

# Đếm số phim từng năm
phim_count_by_year = filtered_df.groupby('nam_bat_dau').size()
print(phim_count_by_year)
# Tạo DataFrame mới lưu trữ các phim hay nhất theo từng thể loại
best_movies_by_genre = merged_df.loc[merged_df.groupby('the_loai')['diem_binh_chon'].idxmax()]

print(best_movies_by_genre[['the_loai', 'ten_phim', 'diem_binh_chon']])